// Copyright, 2021, by Samuel G. D. Williams. <http://www.codeotaku.com>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "backend.h"

static ID id_transfer, id_alive_p;

void Init_Event_Backend(VALUE Event_Backend) {
	id_transfer = rb_intern("transfer");
	id_alive_p = rb_intern("alive?");
}

#if HAVE_RB_FIBER_TRANSFER_KW
#define HAVE_RB_FIBER_TRANSFER 1
#else
#define HAVE_RB_FIBER_TRANSFER 0
#endif

VALUE
Event_Backend_transfer(VALUE fiber) {
#if HAVE_RB_FIBER_TRANSFER
	return rb_fiber_transfer(fiber, 0, NULL);
#else
	return rb_funcall(fiber, id_transfer, 0);
#endif
}

VALUE
Event_Backend_resume_safe(VALUE fiber, VALUE result) {
	// VALUE alive = rb_funcall(fiber, id_alive_p, 0);
	VALUE alive = rb_fiber_alive_p(fiber);
	
	if (RTEST(alive)) {
#if HAVE_RB_FIBER_TRANSFER
		return rb_fiber_transfer(fiber, 1, &result);
#else
		return rb_funcall(fiber, id_transfer, 1, result);
#endif
	} else {
		return Qnil;
	}
}
