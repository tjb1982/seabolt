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


#include "integration.hpp"
#include "catch.hpp"

TEST_CASE("Extract metadata", "[unit]") {
    GIVEN("an open and initialised connection") {
        struct BoltConnection* connection = bolt_open_init_default();
        char key[] = "connection_id";
        BoltValue* metadata = BoltValue_create();
        BoltValue_format_as_Dictionary(metadata, 1);
        BoltDictionary_set_key(metadata, 0, key, strlen(key));
        std::string connection_id;

        WHEN("new connection_id would not overrun buffer") {
            std::string old_connection_id = BoltConnection_id(connection);
            std::string value = "foo";

            BoltValue_format_as_String(
                BoltDictionary_value(metadata, 0), value.c_str(), value.length()
            );
            BoltProtocolV3_extract_metadata(connection, metadata);
            connection_id = BoltConnection_id(connection);

            THEN("it should == the old connection_id + \", \" + new value") {
                REQUIRE(old_connection_id + ", " + value == connection_id);
            }
            THEN("it should not be concatenated to a blank with comma") {
                REQUIRE(connection_id != ", " + value);
            }
        }
        WHEN("new connection_id would overrun buffer") {
            THEN("new connection_id should be ignored and the original connection_id should persist") {
                std::string old_connection_id = BoltConnection_id(connection);
                std::string value =
                    "0123456789""0123456789""0123456789""0123456789""0123456789"
                    "0123456789""0123456789""0123456789""0123456789""0123456789"
                    "0123456789""0123456789""0123456789""0123456789""0123456789"
                    "0123456789""0123456789""0123456789""0123456789""0123456789"
                    ;
                BoltValue_format_as_String(BoltDictionary_value(metadata, 0), value.c_str(), value.length());
                BoltProtocolV3_extract_metadata(connection, metadata);
                connection_id = BoltConnection_id(connection);
                REQUIRE(connection_id.find(value) == std::string::npos);
                REQUIRE(connection_id == old_connection_id);
            }
        }

        BoltValue_destroy(metadata);
        bolt_close_and_destroy_b(connection);
    }
}
