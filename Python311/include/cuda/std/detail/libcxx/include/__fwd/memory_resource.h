//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// SPDX-FileCopyrightText: Copyright (c) 2023 NVIDIA CORPORATION & AFFILIATES.
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCUDACXX___FWD_MEMORY_RESOURCE_H
#define _LIBCUDACXX___FWD_MEMORY_RESOURCE_H

#ifndef __cuda_std__
#include <__config>
#endif // __cuda_std__

#if defined(_LIBCUDACXX_USE_PRAGMA_GCC_SYSTEM_HEADER)
#pragma GCC system_header
#endif

_LIBCUDACXX_BEGIN_NAMESPACE_STD

namespace pmr {
template <class _ValueType>
class _LIBCUDACXX_TEMPLATE_VIS polymorphic_allocator;
} // namespace pmr

_LIBCUDACXX_END_NAMESPACE_STD

#endif // _LIBCUDACXX___FWD_MEMORY_RESOURCE_H
