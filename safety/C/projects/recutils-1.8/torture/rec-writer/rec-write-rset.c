/* -*- mode: C -*-
 *
 *       File:         rec-write-rset.c
 *       Date:         Mon Nov 15 13:56:25 2010
 *
 *       GNU recutils - rec_write_rset unit tests.
 *
 */

/* Copyright (C) 2010-2015 Jose E. Marchesi */

/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <config.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <check.h>

#include <rec.h>

/*-
 * Test: rec_write_rset_nominal
 * Unit: rec_write_rset
 * Description:
 * + Write record sets.
 */
START_TEST(rec_write_rset_nominal)
{
  rec_writer_t writer;
  rec_rset_t rset;
  rec_record_t record;
  rec_field_t field;
  char *str;
  size_t str_size;

  rset = rec_rset_new ();
  fail_if (rset == NULL);

  record = rec_record_new ();
  fail_if (record == NULL);
  field = rec_field_new ("foo1", "value1");
  fail_if (field == NULL);
  fail_if (rec_mset_append (rec_record_mset (record), MSET_FIELD, (void *) field, MSET_ANY) == NULL);
  field = rec_field_new ("foo2", "value2");
  fail_if (field == NULL);
  fail_if (rec_mset_append (rec_record_mset (record), MSET_FIELD, (void *) field, MSET_ANY) == NULL);
  fail_if (rec_mset_append (rec_rset_mset (rset), MSET_RECORD, (void *) record, MSET_ANY) == NULL);

  record = rec_record_new ();
  fail_if (record == NULL);
  field = rec_field_new ("bar1", "value1");
  fail_if (field == NULL);
  fail_if (rec_mset_append (rec_record_mset (record), MSET_FIELD, (void *) field, MSET_ANY) == NULL);
  field = rec_field_new ("bar2", "value2");
  fail_if (field == NULL);
  fail_if (rec_mset_append (rec_record_mset (record), MSET_FIELD, (void *) field, MSET_ANY) == NULL);
  fail_if (rec_mset_append (rec_rset_mset (rset), MSET_RECORD, (void *) record, MSET_ANY) == NULL);
  
  writer = rec_writer_new_str (&str, &str_size);
  rec_writer_set_mode (writer, REC_WRITER_NORMAL);
  fail_if (!rec_write_rset (writer, rset));
  rec_rset_destroy (rset);
  rec_writer_destroy (writer);
  fail_if (strcmp (str,
                   "foo1: value1\nfoo2: value2\n\nbar1: value1\nbar2: value2\n") != 0);
  free (str);
}
END_TEST

/*
 * Test creation function
 */
TCase *
test_rec_write_rset (void)
{
  TCase *tc = tcase_create ("rec_write_rset");
  tcase_add_test (tc, rec_write_rset_nominal);

  return tc;
}

/* End of rec-write-rset.c */
