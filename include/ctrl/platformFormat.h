#ifndef MEMORY_FORMAT_H_
#define MEMORY_FORMAT_H_

#include <endian.h>

#define CTRL_BIG_ENDIAN __BIG_ENDIAN
#define CTRL_LITTLE_ENDIAN __LITTLE_ENDIAN

#if __BYTE_ORDER == __BIG_ENDIAN
#   define CTRL_BYTE_ORDER CTRL_BIG_ENDIAN
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN
#   define CTRL_BYTE_ORDER CTRL_LITTLE_ENDIAN
#endif

struct CheckAlignment { char c; long long l; };
#define CTRL_MEMORY_ALIGNMENT sizeof(CheckAlignment) - sizeof(long long)

#endif // MEMORY_FORMAT_H_

/*
 * Copyright (C) 2010, 2012 by Gerrit Daniels <gerrit.daniels@gmail.com>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
