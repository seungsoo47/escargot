/* Copyright 2019-present Samsung Electronics Co., Ltd. and other contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Copyright 2015 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Tests that lastIndex of a global RegExp is overwritten as per
// ECMA-262 6.0 21.2.5.8 step 10.c.

var global = /./g;
global.lastIndex = { valueOf: function() { assert(false); } };
assert("X" === "x".replace(global, function(a) { return "X"; }));
assert(0 === global.lastIndex);