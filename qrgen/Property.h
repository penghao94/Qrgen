#pragma once
// This is a declaration of getter and setter define by macro
// 
// Copyright (C) 2017 Swanny Peng <ph1994wh@gmail.com>
// 
// This Source Code Form is subject to the terms of the Mozilla Public License 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.
#ifndef PROPERTY_H
#define PROPERTY_H

#define GET(type,name,var)\
	type get##name(){	  \
		return var;       \
	}                     \

#define GET_P(type,name,var)\
	type* get##name(){      \
		return var;         \
	}                       \

#define SET(type,name,var)\
	void set##name(type v){	  \
		 var=v;       \
	}                     \


#define SET_P(type,name,var)\
	void set##name(type* v){\
		var=v;}             \

#define PROPERTY(type,name,var)\
			GET(type,name,var)\
			SET(type,name,var)\

#endif // !PROPERTY_H

