/*
    Copyright (C) 2023 Jean Kieffer

    This file is part of Arb.

    Arb is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "acb.h"

int main(void)
{
    slong iter;
    flint_rand_t state;

    flint_printf("vec_set_real_imag....");
    fflush(stdout);

    flint_randinit(state);

    for (iter = 0; iter < 1000 * flint_test_multiplier(); iter++)
    {
        slong len = n_randint(state, 100);
        slong prec = 10 + n_randint(state, 100);
        slong mag_bits = n_randint(state, 10);

        arb_ptr re, im;
        acb_ptr z, t;
        slong k;

        re = _arb_vec_init(len);
        im = _arb_vec_init(len);
        z = _acb_vec_init(len);
        t = _acb_vec_init(len);

        for (k = 0; k < len; k++)
        {
            acb_randtest_precise(&z[k], state, prec, mag_bits);
        }
        _acb_vec_get_real(re, z, len);
        _acb_vec_get_imag(im, z, len);
        _acb_vec_set_real_imag(t, re, im, len);

        for (k = 0; k < len; k++)
        {
            if (!acb_equal(&z[k], &t[k]))
            {
                flint_printf("FAIL\n\n");
                flint_abort();
            }
        }

        _arb_vec_clear(re, len);
        _arb_vec_clear(im, len);
        _acb_vec_clear(z, len);
        _acb_vec_clear(t, len);
    }

    flint_randclear(state);
    flint_cleanup();
    flint_printf("PASS\n");
    return 0;
}
