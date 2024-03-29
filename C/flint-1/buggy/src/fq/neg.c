/*
    Copyright (C) 2011, 2012 Sebastian Pancratz
    Copyright (C) 2012 Andres Goens
    Copyright (C) 2013 Mike Hansen

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include "fmpz_poly.h"
#include "fmpz_mod_poly.h"
#include "fq.h"

void
fq_neg(fq_t rop, const fq_t op, const fq_ctx_t ctx)
{
    slong len = op->length;
    fmpz_poly_fit_length(rop, len);
    _fmpz_poly_set_length(rop, len);
    _fmpz_mod_poly_neg(rop->coeffs, op->coeffs, op->length, ctx->ctxp);
}
