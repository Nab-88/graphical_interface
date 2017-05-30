//
//  ext_testclass.h
//  projet_c_IG
//
//  Created by François Bérard on 1/6/16.
//  Copyright © 2016 LIG-IIHM. All rights reserved.
//

#ifndef EXT_TESTCLASS_H
#define EXT_TESTCLASS_H

#include "ei_widget.h"


void ext_testclass_register_class(ei_bool_t drawfunc_draws_children);

void ext_testclass_configure(ei_widget_t* widget, int margin);


#endif /* ext_testclass_h */
