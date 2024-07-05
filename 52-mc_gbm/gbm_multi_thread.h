#pragma once
#include "gbm_multi.h"

multipath gbm_multipath_opt_thread(const GBMParam &gbm, const MCParam &mc,
                                   const Market &mkt, const Eval &eval,
                                   const int &n_thread);
