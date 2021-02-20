#include "gc/shenandoah/shenandoahFreeSet.hpp"
#include "gc/shenandoah/shenandoahOldGC.hpp"
#include "gc/shenandoah/shenandoahOopClosures.inline.hpp"

ShenandoahOldGC::ShenandoahOldGC(ShenandoahGeneration* generation) :
  ShenandoahConcurrentGC(generation) {}

bool ShenandoahOldGC::collect(GCCause::Cause cause) {
  ShenandoahHeap* heap = ShenandoahHeap::heap();

  // Continue concurrent mark, do not reset regions, do not mark roots, do not collect $200.
  entry_mark();
  if (check_cancellation_and_abort(ShenandoahDegenPoint::_degenerated_mark)) return false;

  // Complete marking under STW
  vmop_entry_final_mark();

  // We aren't dealing with old generation evacuation yet. Our heuristic
  // should not have built a cset in final mark.
  assert(!heap->is_evacuation_in_progress(), "Old gen evacuations are not supported");

  // Concurrent stack processing
  if (heap->is_evacuation_in_progress()) {
    entry_thread_roots();
  }

  // Process weak roots that might still point to regions that would be broken by cleanup
  if (heap->is_concurrent_weak_root_in_progress()) {
    entry_weak_refs();
    entry_weak_roots();
  }

  // Final mark might have reclaimed some immediate garbage, kick cleanup to reclaim
  // the space. This would be the last action if there is nothing to evacuate.
  entry_cleanup_early();

  {
    ShenandoahHeapLocker locker(heap->lock());
    heap->free_set()->log_status();
  }

  // Perform concurrent class unloading
  if (heap->unload_classes() &&
      heap->is_concurrent_weak_root_in_progress()) {
    entry_class_unloading();
  }

  // Processing strong roots
  // This may be skipped if there is nothing to update/evacuate.
  // If so, strong_root_in_progress would be unset.
  if (heap->is_concurrent_strong_root_in_progress()) {
    entry_strong_roots();
  }

  entry_rendezvous_roots();
  return true;
}
