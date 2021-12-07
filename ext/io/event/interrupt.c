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

static const int DEBUG = 0;

#include "interrupt.h"
#include <fcntl.h>

#ifdef HAVE_SYS_EVENT_H
#include <sys/eventfd.h>
#endif

#ifdef HAVE_SYS_EVENT_H
void IO_Event_Interrupt_open(struct IO_Event_Interrupt *interrupt)
{
	interrupt->descriptor = eventfd2(0, EFD_CLOEXEC | EFD_NONBLOCK);
}

int IO_Event_Interrupt_wait(struct IO_Event_Interrupt *interrupt)
{
	return interrupt->descriptor;
}

void IO_Event_Interrupt_close(struct IO_Event_Interrupt *interrupt)
{
	close(interrupt->descriptor);
}

int IO_Event_Interrupt_signal(struct IO_Event_Interrupt *interrupt)
{
	uint64_t value = 1;
	return write(interrupt->descriptor, &value, sizeof(value));
}

void IO_Event_Interrupt_clear(struct IO_Event_Interrupt *interrupt)
{
	uint64_t value = 0;
	read(interrupt->descriptor, &value, sizeof(value));
}
#else
void IO_Event_Interrupt_open(struct IO_Event_Interrupt *interrupt)
{
	pipe2(interrupt->descriptor, O_CLOEXEC | O_NONBLOCK);
}

int IO_Event_Interrupt_wait(struct IO_Event_Interrupt *interrupt)
{
	return interrupt->descriptor[0];
}

void IO_Event_Interrupt_close(struct IO_Event_Interrupt *interrupt)
{
	close(interrupt->descriptor[0]);
	close(interrupt->descriptor[1]);
}

int IO_Event_Interrupt_signal(struct IO_Event_Interrupt *interrupt)
{
	return write(interrupt->descriptor[1], ".", 1);
}

void IO_Event_Interrupt_clear(struct IO_Event_Interrupt *interrupt)
{
	char buffer[128];
	read(interrupt->descriptor[0], buffer, sizeof(buffer));
	return value;
}
#endif
