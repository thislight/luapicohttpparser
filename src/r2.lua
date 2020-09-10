-- Copyright (C) 2020 thisLight
-- 
-- luapicohttpparser is free software: you can redistribute it and/or modify
-- it under the terms of the GNU Lesser General Public License as published by
-- the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
-- 
-- luapicohttpparser is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU Lesser General Public License for more details.
-- 
-- You should have received a copy of the GNU Lesser General Public License
-- along with luapicohttpparser. If not, see <http://www.gnu.org/licenses/>.
local lphrc = require "lphr.c"

return {
    parse_request = lphrc.parse_request_r2,
    parse_response = lphrc.parse_response_r2,
}
