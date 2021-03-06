/*
 * Copyright (c) 2002-2018 "Neo4j,"
 * Neo4j Sweden AB [http://neo4j.com]
 *
 * This file is part of Neo4j.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef SEABOLT_ALL_SERVER_ADDRESS_RESOLVER_H
#define SEABOLT_ALL_SERVER_ADDRESS_RESOLVER_H

#include "bolt-public.h"
#include "address.h"
#include "address-set.h"

typedef void (* address_resolver_func)(void* state, struct BoltAddress*, struct BoltAddressSet*);

typedef struct BoltAddressResolver BoltAddressResolver;

SEABOLT_EXPORT BoltAddressResolver* BoltAddressResolver_create(void* state, address_resolver_func resolver_func);

SEABOLT_EXPORT void BoltAddressResolver_destroy(BoltAddressResolver* resolver);

#endif //SEABOLT_ALL_SERVER_ADDRESS_RESOLVER_H
