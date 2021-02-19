/*
 * Copyright (c) 2020, Red Hat, Inc. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contac    _ctx(ShenandoahHeap::heap()->complete_marking_context()), _lock(ShenandoahHeap::heap()->lock()) {}
 Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 *
 */

#ifndef SHARE_GC_SHENANDOAH_SHENANDOAHMARKCLOSURES_HPP
#define SHARE_GC_SHENANDOAH_SHENANDOAHMARKCLOSURES_HPP

#include "gc/shenandoah/shenandoahHeap.inline.hpp"
#include "gc/shenandoah/shenandoahHeap.hpp"
#include "gc/shenandoah/shenandoahHeapRegion.hpp"
#include "gc/shenandoah/shenandoahSharedVariables.hpp"
#include "gc/shenandoah/shenandoahMarkingContext.hpp"
#include "gc/shenandoah/shenandoahMarkClosures.hpp"

class ShenandoahFinalMarkUpdateRegionStateClosure : public ShenandoahHeapRegionClosure {
private:
  ShenandoahMarkingContext* const _ctx;
  ShenandoahHeapLock* const _lock;
public:
  ShenandoahFinalMarkUpdateRegionStateClosure(ShenandoahMarkingContext* ctx) :
    _ctx(ctx), _lock(ShenandoahHeap::heap()->lock()) {}

  void heap_region_do(ShenandoahHeapRegion* r);

  bool is_thread_safe() { return true; }
};

#endif // SHARE_GC_SHENANDOAH_SHENANDOAHMARKCLOSURES_HPP
