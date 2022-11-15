#include <stdint.h>
#include <stdio.h>
#include <climits>
#include "Tools/general_utils.h"

#include <fuzzer/FuzzedDataProvider.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    FuzzedDataProvider provider(data, size);
    float esn0 = provider.ConsumeFloatingPoint<float>();
    int upsample_factor = provider.ConsumeIntegralInRange<int>(1, INT_MAX);
    aff3ct::tools::esn0_to_sigma(esn0, upsample_factor);

    return 0;
}