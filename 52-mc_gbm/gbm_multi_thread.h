#pragma once
#include "gbm_multi.h"

multipath gbm_multipath_opt_thread(const GBMParam &gbm, const MCParam &mc,
                                   const Market &mkt, const Eval &eval,
                                   size_t n_thread);
