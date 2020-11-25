/*!
*  \file hd-data.c
*  \brief All data which is composed into the eternal holiday list.
*/
/*
*  Copyright (c) 1994, 95, 96, 1997, 2000, 2011 Thomas Esken
*  Copyright (c) 2010, 2011, 2013 Free Software Foundation, Inc.
*
*  This software doesn't claim completeness, correctness or usability.
*  On principle I will not be liable for ANY damages or losses (implicit
*  or explicit), which result from using or handling my software.
*  If you use this software, you agree without any exception to this
*  agreement, which binds you LEGALLY !!
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the `GNU General Public License' as published by
*  the `Free Software Foundation'; either version 3, or (at your option)
*  any later version.
*
*  You should have received a copy of the `GNU General Public License'
*  along with this program; if not, write to the:
*
*/



/*
*  Include header files.
*/
#include "tailor.h"
#include "common.h"
#include "globals.h"
#include "hd-astro.h"
#include "hd-defs.h"
#include "hd-data1.h"
#if !HD_TOP20CC
# include "hd-data2.h"
# include "hd-data3.h"
# include "hd-data4.h"
# include "hd-data5.h"
#endif
#include "hd-use.h"
#include "tty.h"
#include "utils.h"
#include "hd-data.h"



/*
*  GLOBAL variables definitions.
*/
/*!
   The table used to manage the country specific holidays is a vector of
     `Cc_struct' terminated by an element containing a `cc_info' which is zero!
     The `cc_id' entries in this table *MUST* be listed in ASCENDING SORT ORDER !!
*/
const Cc_struct cc_holidays[] = {
/*
  The A2 (2-letters) country codes are taken from ISO-3166.

  { char *cc_id, char *cc_info, void (*Cc_hdy_handler)() },
*/
#if HD_TOP20CC
  {"AU", N_("Australia"), au_hdy},
  {"BE", N_("Belgium"), be_hdy},
  {"BR", N_("Brazil"), br_hdy},
  {"CA", N_("Canada"), ca_hdy},
  {"CH", N_("Switzerland"), ch_hdy},
  {"CN", N_("China"), cn_hdy},
  {CC_DE, N_("Germany"), de_hdy},
  {"ES", N_("Spain"), es_hdy},
  {"FR", N_("France"), fr_hdy},
  {CC_GB, N_("Great Britain"), gb_hdy},
  {"IL", N_("Israel"), il_hdy},
  {"IT", N_("Italy"), it_hdy},
  {"JP", N_("Japan"), jp_hdy},
  {"KR", N_("Republic of Korea"), kr_hdy},
  {"MX", N_("Mexico"), mx_hdy},
  {"NL", N_("Netherlands"), nl_hdy},
  {"NZ", N_("New Zealand"), nz_hdy},
  {"PT", N_("Portugal"), pt_hdy},
  {"RU", N_("Russian Federation"), ru_hdy},
  {CC_US, N_("United States"), us_hdy},
#else /* !HD_TOP20CC */
  {"AD", N_("Andorra"), ad_hdy},
  {"AE", N_("United Arab Emirates"), ae_hdy},
  {"AF", N_("Afghanistan"), af_hdy},
  {"AG", N_("Antigua and Barbuda"), ag_hdy},
  {"AI", N_("Anguilla"), ai_hdy},
  {"AL", N_("Albania"), al_hdy},
  {"AM", N_("Armenia"), am_hdy},
  {"AN_BO", N_("Netherlands Antilles/Bonaire"), an_bo_hdy},
  {"AN_CU", N_("Netherlands Antilles/Curaçao"), an_cu_hdy},
  {"AN_MA", N_("Netherlands Antilles/St Maarten"), an_ma_hdy},
  {"AN_SA", N_("Netherlands Antilles/Saba and Statia"), an_sa_hdy},
  {"AO", N_("Angola #"), ao_hdy},
  {"AR", N_("Argentina"), ar_hdy},
  {"AS", N_("American Samoa"), as_hdy},
  {"AT", N_("Austria"), at_hdy},
  {"AU_CT", N_("Australia/Canberra"), au_ct_hdy},
  {"AU_NT", N_("Australia/Northern Territory"), au_nt_hdy},
  {"AU_QU", N_("Australia/Queensland"), au_qu_hdy},
  {"AU_SA", N_("Australia/Southern Australia"), au_sa_hdy},
  {"AU_SW", N_("Australia/New South Wales"), au_sw_hdy},
  {"AU_TA", N_("Australia/Tasmania"), au_ta_hdy},
  {"AU_VI", N_("Australia/Victoria"), au_vi_hdy},
  {"AU_WA", N_("Australia/Western Australia"), au_wa_hdy},
  {"AW", N_("Aruba"), aw_hdy},
  {"AZ", N_("Azerbaijan"), az_hdy},
  {"BA", N_("Bosnia-Herzegovina"), ba_hdy},
  {"BB", N_("Barbados"), bb_hdy},
  {"BD", N_("Bangladesh #"), bd_hdy},
  {"BE", N_("Belgium"), be_hdy},
  {"BF", N_("Burkina Faso"), bf_hdy},
  {"BG", N_("Bulgaria"), bg_hdy},
  {"BH", N_("Bahrain"), bh_hdy},
  {"BI", N_("Burundi #"), bi_hdy},
  {"BJ", N_("Benin #"), bj_hdy},
  {"BM", N_("Bermuda"), bm_hdy},
  {"BN", N_("Brunei"), bn_hdy},
  {"BO", N_("Bolivia"), bo_hdy},
  {"BR", N_("Brazil"), br_hdy},
  {"BS", N_("Bahamas"), bs_hdy},
  {"BT", N_("Bhutan #"), bt_hdy},
  {"BV", N_("Bouvet Island"), bv_hdy},
  {"BW", N_("Botswana"), bw_hdy},
  {"BY", N_("Belarus"), by_hdy},
  {"BZ", N_("Belize"), bz_hdy},
  {"CA_AL", N_("Canada/Alberta"), ca_al_hdy},
  {"CA_BC", N_("Canada/British Columbia"), ca_bc_hdy},
  {"CA_MA", N_("Canada/Manitoba"), ca_ma_hdy},
  {"CA_NB", N_("Canada/New Brunswick"), ca_nb_hdy},
  {"CA_NF", N_("Canada/Newfoundland and Labrador"), ca_nf_hdy},
  {"CA_NS", N_("Canada/Nova Scotia"), ca_ns_hdy},
  {"CA_NW", N_("Canada/Northwest Territories"), ca_nw_hdy},
  {"CA_ON", N_("Canada/Ontario"), ca_on_hdy},
  {"CA_PE", N_("Canada/Prince Edward Island"), ca_pe_hdy},
  {"CA_QU", N_("Canada/Québec"), ca_qu_hdy},
  {"CA_SA", N_("Canada/Saskatchewan"), ca_sa_hdy},
  {"CA_YU", N_("Canada/Yukon"), ca_yu_hdy},
  {"CC", N_("Cocos Islands (Keeling)"), cc_hdy},
  {"CD", N_("Democratic Republic of Congo #"), cd_hdy},
  {"CF", N_("Central African Republic #"), cf_hdy},
  {"CG", N_("Republic of Congo #"), cg_hdy},
  {"CH_AG", N_("Switzerland/Aargau"), ch_ag_hdy},
  {"CH_AI", N_("Switzerland/Appenzell Innerrhoden"), ch_ai_hdy},
  {"CH_AR", N_("Switzerland/Appenzell Ausserrhoden"), ch_ar_hdy},
  {"CH_BE", N_("Switzerland/Bern"), ch_be_hdy},
  {"CH_BL", N_("Switzerland/Basel-Landschaft"), ch_bl_hdy},
  {"CH_BS", N_("Switzerland/Basel-Stadt"), ch_bs_hdy},
  {"CH_FR", N_("Switzerland/Fribourg"), ch_fr_hdy},
  {"CH_GE", N_("Switzerland/Genève"), ch_ge_hdy},
  {"CH_GL", N_("Switzerland/Glarus"), ch_gl_hdy},
  {"CH_GR", N_("Switzerland/Graubünden"), ch_gr_hdy},
  {"CH_JU", N_("Switzerland/Jura"), ch_ju_hdy},
  {"CH_LU", N_("Switzerland/Luzern"), ch_lu_hdy},
  {"CH_NE", N_("Switzerland/Neuchâtel"), ch_ne_hdy},
  {"CH_NW", N_("Switzerland/Nidwalden"), ch_nw_hdy},
  {"CH_OW", N_("Switzerland/Obwalden"), ch_ow_hdy},
  {"CH_SG", N_("Switzerland/St Gallen"), ch_sg_hdy},
  {"CH_SH", N_("Switzerland/Schaffhausen"), ch_sh_hdy},
  {"CH_SO", N_("Switzerland/Solothurn"), ch_so_hdy},
  {"CH_SZ", N_("Switzerland/Schwyz"), ch_sz_hdy},
  {"CH_TG", N_("Switzerland/Thurgau"), ch_tg_hdy},
  {"CH_TI", N_("Switzerland/Ticino"), ch_ti_hdy},
  {"CH_UR", N_("Switzerland/Uri"), ch_ur_hdy},
  {"CH_VD", N_("Switzerland/Vaud"), ch_vd_hdy},
  {"CH_VS", N_("Switzerland/Valais"), ch_vs_hdy},
  {"CH_ZG", N_("Switzerland/Zug"), ch_zg_hdy},
  {"CH_ZH", N_("Switzerland/Zürich"), ch_zh_hdy},
  {"CI", N_("Côte d'Ivoire"), ci_hdy},
  {"CK", N_("Cook Islands"), ck_hdy},
  {"CL", N_("Chile"), cl_hdy},
  {"CM", N_("Cameroon"), cm_hdy},
  {"CN", N_("China"), cn_hdy},
  {"CO", N_("Colombia"), co_hdy},
  {"CR", N_("Costa Rica"), cr_hdy},
  {"CU", N_("Cuba"), cu_hdy},
  {"CV", N_("Cape Verde"), cv_hdy},
  {"CX", N_("Christmas Islands"), cx_hdy},
  {"CY", N_("Cyprus"), cy_hdy},
  {"CZ", N_("Czech Republic"), cz_hdy},
  {"DE_BB", N_("Germany/Brandenburg"), de_bb_hdy},
  {"DE_BE", N_("Germany/Berlin"), de_be_hdy},
  {"DE_BW", N_("Germany/Baden-Württemberg"), de_bw_hdy},
  {"DE_BY", N_("Germany/Bavaria"), de_by_hdy},
  {"DE_HB", N_("Germany/Bremen"), de_hb_hdy},
  {"DE_HE", N_("Germany/Hesse"), de_he_hdy},
  {"DE_HH", N_("Germany/Hamburg"), de_hh_hdy},
  {"DE_MV", N_("Germany/Mecklenburg-West Pomerania"), de_mv_hdy},
  {"DE_NI", N_("Germany/Lower Saxony"), de_ni_hdy},
  {"DE_NW", N_("Germany/North Rhine-Westphalia"), de_nw_hdy},
  {"DE_RP", N_("Germany/Rhineland Palatinate"), de_rp_hdy},
  {"DE_SH", N_("Germany/Schleswig-Holstein"), de_sh_hdy},
  {"DE_SL", N_("Germany/Saarland"), de_sl_hdy},
  {"DE_SN", N_("Germany/Saxony"), de_sn_hdy},
  {"DE_ST", N_("Germany/Saxony-Anhalt"), de_st_hdy},
  {"DE_TH", N_("Germany/Thuringia"), de_th_hdy},
  {"DJ", N_("Djibouti"), dj_hdy},
  {"DK", N_("Denmark"), dk_hdy},
  {"DM", N_("Dominica"), dm_hdy},
  {"DO", N_("Dominican Republic"), do_hdy},
  {"DZ", N_("Algeria"), dz_hdy},
  {"EC", N_("Ecuador"), ec_hdy},
  {"EE", N_("Estonia"), ee_hdy},
  {"EG", N_("Egypt"), eg_hdy},
  {"EH", N_("Western Sahara"), eh_hdy},
  {"ER", N_("Eritrea"), er_hdy},
  {"ES", N_("Spain"), es_hdy},
  {"ET", N_("Ethiopia"), et_hdy},
  {"FI", N_("Finland"), fi_hdy},
  {"FJ", N_("Fiji #"), fj_hdy},
  {"FK", N_("Falkland Islands (Malvinas)"), fk_hdy},
  {"FM", N_("Federated States of Micronesia"), fm_hdy},
  {"FO", N_("Faroes"), fo_hdy},
  {"FR", N_("France"), fr_hdy},
  {"GA", N_("Gabon #"), ga_hdy},
  {"GB_EN", N_("Great Britain/England and Wales"), gb_en_hdy},
  {"GB_NI", N_("Great Britain/Northern Ireland"), gb_ni_hdy},
  {"GB_SL", N_("Great Britain/Scotland"), gb_sl_hdy},
  {"GD", N_("Grenada"), gd_hdy},
  {"GE", N_("Georgia"), ge_hdy},
  {"GF", N_("French Guiana"), gf_hdy},
  {"GH", N_("Ghana"), gh_hdy},
  {"GI", N_("Gibraltar"), gi_hdy},
  {"GL", N_("Greenland"), gl_hdy},
  {"GM", N_("Gambia"), gm_hdy},
  {"GN", N_("Guinea"), gn_hdy},
  {"GP", N_("Guadeloupe"), gp_hdy},
  {"GQ", N_("Equatorial Guinea"), gq_hdy},
  {"GR", N_("Greece"), gr_hdy},
  {"GS", N_("South Georgia and South Sandwich Islands"), gs_hdy},
  {"GT", N_("Guatemala"), gt_hdy},
  {"GU", N_("Guam"), gu_hdy},
  {"GW", N_("Guinea-Bissau #"), gw_hdy},
  {"GY", N_("Guyana #"), gy_hdy},
  {"HK", N_("Hong Kong"), hk_hdy},
  {"HM", N_("Heard and Mc Donald Islands"), hm_hdy},
  {"HN", N_("Honduras"), hn_hdy},
  {"HR", N_("Croatia"), hr_hdy},
  {"HT", N_("Haiti"), ht_hdy},
  {"HU", N_("Hungary"), hu_hdy},
  {"ID", N_("Indonesia #"), id_hdy},
  {"IE", N_("Ireland"), ie_hdy},
  {"IL", N_("Israel"), il_hdy},
  {"IN", N_("India #"), in_hdy},
  {"IQ", N_("Iraq"), iq_hdy},
  {"IR", N_("Islamic Republic of Iran"), ir_hdy},
  {"IS", N_("Iceland"), is_hdy},
  {"IT", N_("Italy"), it_hdy},
  {"JM", N_("Jamaica"), jm_hdy},
  {"JO", N_("Jordan"), jo_hdy},
  {"JP", N_("Japan"), jp_hdy},
  {"KE", N_("Kenya #"), ke_hdy},
  {"KG", N_("Kyrgyzstan"), kg_hdy},
  {"KH", N_("Cambodia #"), kh_hdy},
  {"KI", N_("Kiribati #"), ki_hdy},
  {"KM", N_("Comoros"), km_hdy},
  {"KN", N_("St Kitts and Nevis"), kn_hdy},
  {"KP", N_("Democratic People's Republic of Korea"), kp_hdy},
  {"KR", N_("Republic of Korea"), kr_hdy},
  {"KW", N_("Kuwait"), kw_hdy},
  {"KY", N_("Cayman Islands"), ky_hdy},
  {"KZ", N_("Kazakhstan"), kz_hdy},
  {"LA", N_("Laos People's Democratic Republic #"), la_hdy},
  {"LB", N_("Lebanon"), lb_hdy},
  {"LC", N_("St Lucia"), lc_hdy},
  {"LI", N_("Liechtenstein"), li_hdy},
  {"LK", N_("Sri Lanka #"), lk_hdy},
  {"LR", N_("Liberia"), lr_hdy},
  {"LS", N_("Lesotho"), ls_hdy},
  {"LT", N_("Lithuania"), lt_hdy},
  {"LU", N_("Luxembourg"), lu_hdy},
  {"LV", N_("Latvia"), lv_hdy},
  {"LY", N_("Libyan Arab Jamahiriya (Libya)"), ly_hdy},
  {"MA", N_("Morocco"), ma_hdy},
  {"MC", N_("Monaco"), mc_hdy},
  {"MD", N_("Republic of Moldova"), md_hdy},
  {"MG", N_("Madagascar #"), mg_hdy},
  {"MH", N_("Marshall Islands"), mh_hdy},
  {"MK", N_("Former Yugoslav Republic of Macedonia"), mk_hdy},
  {"ML", N_("Mali"), ml_hdy},
  {"MN", N_("Mongolia #"), mn_hdy},
  {"MO", N_("Macau"), mo_hdy},
  {"MP", N_("Northern Marian Islands (Saipan)"), mp_hdy},
  {"MQ", N_("Martinique"), mq_hdy},
  {"MR", N_("Mauritania"), mr_hdy},
  {"MS", N_("Montserrat"), ms_hdy},
  {"MT", N_("Malta"), mt_hdy},
  {"MU", N_("Mauritius"), mu_hdy},
  {"MV", N_("Maldives"), mv_hdy},
  {"MW", N_("Malawi"), mw_hdy},
  {"MX", N_("Mexico"), mx_hdy},
  {"MY", N_("Malaysia #"), my_hdy},
  {"MZ", N_("Mozambique"), mz_hdy},
  {"NA", N_("Namibia"), na_hdy},
  {"NC", N_("New Caledonia"), nc_hdy},
  {"NE", N_("Niger"), ne_hdy},
  {"NF", N_("Norfolk Island"), nf_hdy},
  {"NG", N_("Nigeria"), ng_hdy},
  {"NI", N_("Nicaragua"), ni_hdy},
  {"NL", N_("Netherlands"), nl_hdy},
  {"NM", N_("Myanmar (Burma) #"), nm_hdy},
  {"NO", N_("Norway"), no_hdy},
  {"NP", N_("Nepal #"), np_hdy},
  {"NR", N_("Nauru"), nr_hdy},
  {"NU", N_("Niue"), nu_hdy},
  {"NZ", N_("New Zealand"), nz_hdy},
  {"OM", N_("Oman"), om_hdy},
  {"PA", N_("Panama"), pa_hdy},
  {"PE", N_("Peru"), pe_hdy},
  {"PF", N_("French Polynesia"), pf_hdy},
  {"PG", N_("Papua New Guinea"), pg_hdy},
  {"PH", N_("Philippines"), ph_hdy},
  {"PK", N_("Pakistan"), pk_hdy},
  {"PL", N_("Poland"), pl_hdy},
  {"PM", N_("Saint-Pierre and Miquelon"), pm_hdy},
  {"PN", N_("Pitcairn"), pn_hdy},
  {"PR", N_("Puerto Rico"), pr_hdy},
  {"PT", N_("Portugal"), pt_hdy},
  {"PW", N_("Palau"), pw_hdy},
  {"PY", N_("Paraguay"), py_hdy},
  {"QA", N_("Qatar"), qa_hdy},
  {"RE", N_("Réunion"), re_hdy},
  {"RO", N_("Romania"), ro_hdy},
  {"RU", N_("Russian Federation"), ru_hdy},
  {"RW", N_("Rwanda"), rw_hdy},
  {"SA", N_("Saudi Arabia"), sa_hdy},
  {"SB", N_("Solomon Islands"), sb_hdy},
  {"SC", N_("Seychellen"), sc_hdy},
  {"SD", N_("Sudan"), sd_hdy},
  {"SE", N_("Sweden"), se_hdy},
  {"SG", N_("Singapore #"), sg_hdy},
  {"SH", N_("St Helena"), sh_hdy},
  {"SI", N_("Slovenia"), si_hdy},
  {"SJ", N_("Svalbard and Jan Mayen Islands"), sj_hdy},
  {"SK", N_("Slovakia"), sk_hdy},
  {"SL", N_("Sierra Leone"), sl_hdy},
  {"SM", N_("San Marino"), sm_hdy},
  {"SN", N_("Senegal"), sn_hdy},
  {"SO", N_("Somalia"), so_hdy},
  {"SR", N_("Suriname #"), sr_hdy},
  {"ST", N_("Sao Tomé and Principe"), st_hdy},
  {"SV", N_("El Salvador"), sv_hdy},
  {"SY", N_("Syrian Arab Republic (Syria)"), sy_hdy},
  {"SZ", N_("Swaziland"), sz_hdy},
  {"TC", N_("Turks and Caicos Islands"), tc_hdy},
  {"TD", N_("Chad"), td_hdy},
  {"TG", N_("Togo #"), tg_hdy},
  {"TH", N_("Thailand #"), th_hdy},
  {"TJ", N_("Tajikistan"), tj_hdy},
  {"TK", N_("Tokelau"), tk_hdy},
  {"TM", N_("Turkmenistan"), tm_hdy},
  {"TN", N_("Tunisia"), tn_hdy},
  {"TO", N_("Tonga"), to_hdy},
  {"TR", N_("Turkey"), tr_hdy},
  {"TT", N_("Trinidad and Tobago #"), tt_hdy},
  {"TV", N_("Tuvalu"), tv_hdy},
  {"TW", N_("Taiwan"), tw_hdy},
  {"TZ", N_("Tanzania"), tz_hdy},
  {"UA", N_("Ukraine"), ua_hdy},
  {"UG", N_("Uganda"), ug_hdy},
  {"US_AK", N_("United States/Alaska"), us_ak_hdy},
  {"US_AL", N_("United States/Alabama"), us_al_hdy},
  {"US_AR", N_("United States/Arkansas"), us_ar_hdy},
  {"US_AZ", N_("United States/Arizona"), us_az_hdy},
  {"US_CA", N_("United States/California"), us_ca_hdy},
  {"US_CO", N_("United States/Colorado"), us_co_hdy},
  {"US_CT", N_("United States/Connecticut"), us_ct_hdy},
  {"US_DC", N_("United States/District of Columbia"), us_dc_hdy},
  {"US_DE", N_("United States/Delaware"), us_de_hdy},
  {"US_FL", N_("United States/Florida"), us_fl_hdy},
  {"US_GA", N_("United States/Georgia"), us_ga_hdy},
  {"US_HI", N_("United States/Hawaii"), us_hi_hdy},
  {"US_IA", N_("United States/Iowa"), us_ia_hdy},
  {"US_ID", N_("United States/Idaho"), us_id_hdy},
  {"US_IL", N_("United States/Illinois"), us_il_hdy},
  {"US_IN", N_("United States/Indiana"), us_in_hdy},
  {"US_KS", N_("United States/Kansas"), us_ks_hdy},
  {"US_KY", N_("United States/Kentucky"), us_ky_hdy},
  {"US_LA", N_("United States/Louisiana"), us_la_hdy},
  {"US_MA", N_("United States/Massachusetts"), us_ma_hdy},
  {"US_MD", N_("United States/Maryland"), us_md_hdy},
  {"US_ME", N_("United States/Maine"), us_me_hdy},
  {"US_MI", N_("United States/Michigan"), us_mi_hdy},
  {"US_MN", N_("United States/Minnesota"), us_mn_hdy},
  {"US_MO", N_("United States/Missouri"), us_mo_hdy},
  {"US_MS", N_("United States/Mississippi"), us_ms_hdy},
  {"US_MT", N_("United States/Montana"), us_mt_hdy},
  {"US_NC", N_("United States/North Carolina"), us_nc_hdy},
  {"US_ND", N_("United States/North Dakota"), us_nd_hdy},
  {"US_NE", N_("United States/Nebraska"), us_ne_hdy},
  {"US_NH", N_("United States/New Hampshire"), us_nh_hdy},
  {"US_NJ", N_("United States/New Jersey"), us_nj_hdy},
  {"US_NM", N_("United States/New Mexico"), us_nm_hdy},
  {"US_NV", N_("United States/Nevada"), us_nv_hdy},
  {"US_NY", N_("United States/New York"), us_ny_hdy},
  {"US_OH", N_("United States/Ohio"), us_oh_hdy},
  {"US_OK", N_("United States/Oklahoma"), us_ok_hdy},
  {"US_OR", N_("United States/Oregon"), us_or_hdy},
  {"US_PA", N_("United States/Pennsylvania"), us_pa_hdy},
  {"US_RI", N_("United States/Rhode Island"), us_ri_hdy},
  {"US_SC", N_("United States/South Carolina"), us_sc_hdy},
  {"US_SD", N_("United States/South Dakota"), us_sd_hdy},
  {"US_TN", N_("United States/Tennessee"), us_tn_hdy},
  {"US_TX", N_("United States/Texas"), us_tx_hdy},
  {"US_UT", N_("United States/Utah"), us_ut_hdy},
  {"US_VA", N_("United States/Virginia"), us_va_hdy},
  {"US_VT", N_("United States/Vermont"), us_vt_hdy},
  {"US_WA", N_("United States/Washington"), us_wa_hdy},
  {"US_WI", N_("United States/Wisconsin"), us_wi_hdy},
  {"US_WV", N_("United States/West Virginia"), us_wv_hdy},
  {"US_WY", N_("United States/Wyoming"), us_wy_hdy},
  {"UY", N_("Uruguay"), uy_hdy},
  {"UZ", N_("Uzbekistan"), uz_hdy},
  {"VC", N_("St Vincent and Grenadines"), vc_hdy},
  {"VE", N_("Venezuela"), ve_hdy},
  {"VG", N_("British Virgin Islands"), vg_hdy},
  {"VI", N_("U.S. Virgin Islands"), vi_hdy},
  {"VN", N_("Viet Nam"), vn_hdy},
  {"VU", N_("Vanuatu"), vu_hdy},
  {"WF", N_("Wallis and Futuna Islands"), wf_hdy},
  {"WS", N_("Samoa"), ws_hdy},
  {"YE", N_("Yemen"), ye_hdy},
  {"YT", N_("Mayotte"), yt_hdy},
  {"YU", N_("Serbia and Montenegro"), yu_hdy},
  {"ZA", N_("South Africa"), za_hdy},
  {"ZM", N_("Zambia"), zm_hdy},
  {"ZW", N_("Zimbabwe"), zw_hdy},
#endif /* !HD_TOP20CC */
  {NULL, NULL, NULL}
};

/*! U.S. holidays observance mode. */
int observe_us_hdy = SAFR_SUMO;

/*! The ISO-3166 character code used in the country specific `hdy_*()' functions. */
char *ptr_cc_id = (char *) NULL;

/*! Toggle which indicates to use a different CC in output. */
Bool use_other_cc = FALSE;

/*!
   The table which contains all the holiday texts
     is an unterminated vector of `Ht_struct'.
*/
const Ht_struct hd_text[] = {
/*
  { int ht_id, char *ht_text },
*/
  /*
   *** Translators, please keep in mind to translate all trailing
   *** holiday texts to texts which have a maximum length of:
   *** 34 characters (if the COUNTRY-CODE suffix has 2 characters)
   *** 33 characters (if the COUNTRY-CODE suffix has 3 characters)
   *** 32 characters (if the COUNTRY-CODE suffix has 4 characters)
   *** 31 characters (if the COUNTRY-CODE_TERRITORY suffix has 5 characters).
   */
  {HD_1ST_ADVENT, N_("1st Advent")},
  {HD_1ST_SUNDAY_IN_LENT, N_("1st Sunday in Lent")},
  {HD_2ND_ADVENT, N_("2nd Advent")},
  {HD_2ND_SUNDAY_IN_LENT, N_("2nd Sunday in Lent")},
  {HD_3RD_ADVENT, N_("3rd Advent")},
  {HD_3RD_SUNDAY_IN_LENT, N_("3rd Sunday in Lent")},
  {HD_4TH_ADVENT, N_("4th Advent")},
  {HD_4TH_SUNDAY_IN_LENT, N_("4th Sunday in Lent")},
  {HD_7_5_3_FESTIVAL, N_("7-5-3 Festival")},
  {HD_ABORIGINES_DAY, N_("Aborigines Day")},
  {HD_ALL_FOOLS_DAY, N_("All Fool's Day")},
  {HD_ALL_SAINTS_DAY, N_("All Saints' Day")},
  {HD_ALL_SOULS_DAY, N_("All Souls' Day")},
  {HD_ALPHABET_DAY, N_("Alphabet Day")},
  {HD_ANNIVERSARY_OF_AUCKLAND, N_("Anniversary of Auckland")},
  {HD_ANNIVERSARY_OF_CANTERBURY_NORTH, N_("Anniversary of Canterbury North")},
  {HD_ANNIVERSARY_OF_CANTERBURY_SOUTH, N_("Anniversary of Canterbury South")},
  {HD_ANNIVERSARY_OF_CHATHAM_ISLANDS, N_("Anniversary of Chatham Islands")},
  {HD_ANNIVERSARY_OF_HAWKES_BAY, N_("Anniversary of Hawkes' Bay")},
  {HD_ANNIVERSARY_OF_MARLBOROUGH, N_("Anniversary of Marlborough")},
  {HD_ANNIVERSARY_OF_NELSON, N_("Anniversary of Nelson")},
  {HD_ANNIVERSARY_OF_OTAGO, N_("Anniversary of Otago")},
  {HD_ANNIVERSARY_OF_SOUTHLAND, N_("Anniversary of Southland")},
  {HD_ANNIVERSARY_OF_TARANAKI, N_("Anniversary of Taranaki")},
  {HD_ANNIVERSARY_OF_WELLINGTON, N_("Anniversary of Wellington")},
  {HD_ANNIVERSARY_OF_WESTLAND, N_("Anniversary of Westland")},
  {HD_ANZAC_DAY, N_("Anzac Day")},
  {HD_ARBOR_DAY, N_("Arbor Day")},
  {HD_ARMED_FORCES_DAY, N_("Armed Forces Day")},
  {HD_ARMISTICE_DAY, N_("Armistice Day")},
  {HD_ASH_MONDAY, N_("Ash Monday")},
  {HD_ASH_WEDNESDAY, N_("Ash Wednesday")},
  {HD_BAHAI_ASCENSION_OF_ABDUL_BAHA, N_("Ascension of Abdu'l-Baha")},
  {HD_BAHAI_ASCENSION_OF_BAHA_ULLAH, N_("Ascension of Baha'u'llah")},
  {HD_BAHAI_BIRTH_OF_BAHA_ULLAH, N_("Birth of Baha'u'llah")},
  {HD_BAHAI_BIRTH_OF_THE_BAB, N_("Birth of the Bab")},
  {HD_BAHAI_DAY_OF_THE_COVENANT, N_("Day of the Covenant")},
  {HD_BAHAI_DECLARATION_OF_THE_BAB, N_("Declaration of the Bab")},
  {HD_BAHAI_FIRST_DAY_OF_RIDVAN, N_("First Day of Ridvan")},
  {HD_BAHAI_MARTYRDOM_OF_THE_BAB, N_("Martyrdom of the Bab")},
  {HD_BAHAI_NEW_YEARS_DAY, N_("Bahá'i New Year's Day")},
  {HD_BAHAI_NINTH_DAY_OF_RIDVAN, N_("Ninth Day of Ridvan")},
  {HD_BAHAI_TWELFTH_DAY_OF_RIDVAN, N_("Twelfth Day of Ridvan")},
  {HD_BANK_HOLIDAY, N_("Bank Holiday")},
  {HD_BASQUE_NATIONAL_DAY, N_("Basque National Day")},
  {HD_BATTLE_OF_BOYNE, N_("Battle of Boyne")},
  {HD_BATTLE_OF_PUEBLA, N_("Battle of Puebla")},
  {HD_BEGINNING_OF_FINANCIAL_YEAR, N_("Beginning of Financial Year")},
  {HD_BENITO_JUAREZ_DAY, N_("Benito Juárez Day")},
  {HD_BODHIDHARMAS_BIRTHDAY, N_("Bodhidharma's Birthday")},
  {HD_BOSSES_DAY, N_("Bosses' Day")},
  {HD_BOXING_DAY, N_("Boxing Day")},
  {HD_BRINGING_HOME_THE_HERDS_DAY, N_("Bringing Home the Herds Day")},
  {HD_BRINGING_IN_THE_HARVEST_DAY, N_("Bringing in the Harvest Day")},
  {HD_BUDDHAS_BIRTHDAY, N_("Buddha's Birthday")},
  {HD_BURNS_NIGHT, N_("Burns Night")},
  {HD_CAMOES_DAY, N_("Camoes Day")},
  {HD_CARNIVAL, N_("Carnival")},
  {HD_CELTIC_BELTANE, N_("Beltane")},
  {HD_CELTIC_IMBOLG, N_("Imbolg")},
  {HD_CELTIC_LUGHNASA, N_("Lughnasa")},
  {HD_CELTIC_SAMHAIN, N_("Samhain/Celtic New Year's Day")},
  {HD_CHILDRENS_DAY, N_("Children's Day")},
  {HD_CHINESE_CYCLE, N_("Cycle")},
  {HD_CHINESE_NEW_YEARS_DAY, N_("Chinese New Year's Day")},
  {HD_CHINESE_NEW_YEARS_EVE, N_("Chinese New Year's Eve")},
  {HD_CHRISTMAS_DAY, N_("Christmas Day")},
  {HD_CHRISTMAS_EVE, N_("Christmas Eve")},
  {HD_CHRISTS_ASCENSION_DAY, N_("Christ's Ascension Day")},
  {HD_CHRYSANTHENUM_DAY, N_("Chrysanthenum Day")},
  {HD_CITIZENSHIP_DAY, N_("Citizenship Day")},
  {HD_CIVIC_HOLIDAY, N_("Civic Holiday")},
  {HD_COLUMBUS_DAY, N_("Columbus Day")},
  {HD_COMING_OF_AGE_DAY, N_("Coming of Age Day")},
  {HD_CONFUCIUS_BIRTHDAY, N_("Confucius' Birthday")},
  {HD_CONSTITUTION_ANNIVERSARY, N_("Constitution Anniversary")},
  {HD_COPTIC_NEW_YEARS_DAY, N_("Coptic New Year's Day")},
  {HD_COSMONAUTS_DAY, N_("Cosmonauts' Day")},
  {HD_CROSS_RAISING_DAY, N_("Cross Raising Day")},
  {HD_CULTURE_DAY, N_("Culture Day")},
  {HD_DAY_AFTER_MID_AUTUMN_FESTIVAL, N_("Day after Mid-Autumn Festival")},
  {HD_DAY_AFTER_TOMB_SWEEPING_DAY, N_("Day after Tomb-Sweeping Day")},
  {HD_DAY_OF_ANDALUCIA, N_("Day of Andalucia")},
  {HD_DAY_OF_CATALONIA, N_("Day of Catalonia")},
  {HD_DAY_OF_GALICIA, N_("Day of Galicia")},
  {HD_DAY_OF_PRAYER_AND_REPENTANCE, N_("Day of Prayer and Repentance")},
  {HD_DAY_OF_RECONCILIATION, N_("Day of Reconciliation")},
  {HD_DEFENDERS_OF_MOTHERLAND_DAY, N_("Defenders of Motherland Day")},
  {HD_DOUBLE_9_DAY, N_("Double-9 Day")},
  {HD_DRAGON_BOAT_FESTIVAL, N_("Dragon Boat Festival")},
  {HD_DYNASTY_DAY, N_("Dynasty Day")},
  {HD_EASTER_MONDAY, N_("Easter Monday")},
  {HD_EASTER_SUNDAY, N_("Easter Sunday")},
  {HD_ECLIPSE_LUNAR, N_("Lunar Eclipse")},
  {HD_ECLIPSE_SOLAR, N_("Solar Eclipse")},
  {HD_ECLIPSE_TYPE_ANNULAR, N_("Annular")},
  {HD_ECLIPSE_TYPE_PARTIAL, N_("Partial")},
  {HD_ECLIPSE_TYPE_PENUMBRAL, N_("Penumbral")},
  {HD_ECLIPSE_TYPE_TOTAL, N_("Total")},
  {HD_EINDHOVENS_LIBERATION, N_("Eindhoven's Liberation")},
  {HD_ELECTION_DAY, N_("Election Day")},
  {HD_EMPERORS_BIRTHDAY, N_("Emperor's Birthday")},
  {HD_EQUINOX_DAY, N_("Equinox Day")},
  {HD_ETHIOPIC_NEW_YEARS_DAY, N_("Ethiopic New Year's Day")},
  {HD_FATHERS_DAY, N_("Father's Day")},
  {HD_FEAST_OF_CORPUS_CHRISTI, N_("Feast of Corpus Christi")},
  {HD_FEAST_OF_FORTUNE, N_("Feast of Fortune")},
  {HD_FEAST_OF_HEART_JESUS, N_("Feast of Heart Jesus")},
  {HD_FESTIVAL_OF_LANTERNS, N_("Festival of Lanterns")},
  {HD_FESTIVAL_OF_SADEH, N_("Festival of Sadeh")},
  {HD_FLAG_DAY, N_("Flag Day")},
  {HD_FLEMISH_CULTURE_DAY, N_("Flemish Culture Day")},
  {HD_FOUNDING_OF_THE_COMMUNIST_PARTY, N_("Founding of the Communist Party")},
  {HD_FRENCH_CULTURE_DAY, N_("French Culture Day")},
  {HD_FRENCH_REVOLUTIONARY, N_("French Revolutionary")},
  {HD_FRIENDSHIP_DAY, N_("Friendship Day")},
  {HD_GERMAN_UNITY_DAY, N_("German Unity Day")},
  {HD_GOOD_FRIDAY, N_("Good Friday")},
  {HD_GOOD_SATURDAY, N_("Good Saturday/Easter Eve")},
  {HD_GRANDPARENTS_DAY, N_("Grandparents' Day")},
  {HD_GREENERY_DAY, N_("Greenery Day")},
  {HD_GROTTO_DAY, N_("Grotto Day")},
  {HD_GROUNDHOG_DAY, N_("Groundhog Day")},
  {HD_GUADALUPE_VIRGINS_DAY, N_("Guadalupe Virgin's Day")},
  {HD_GUY_FAWKES_DAY, N_("Guy Fawkes Day")},
  {HD_HALLOWEEN, N_("Halloween")},
  {HD_HEALTH_AND_SPORTS_DAY, N_("Health and Sport's Day")},
  {HD_HEB_HANNUKAH, N_("Hannukah/Festival of Lights")},
  {HD_HEB_LAG_BOMER, N_("Lag B'Omer")},
  {HD_HEB_PESACH, N_("Pesach/Passover")},
  {HD_HEB_PURIM, N_("Purim/Feast of Lots")},
  {HD_HEB_ROSH_HASHANA, N_("Rosh Hashana/New Year's Day")},
  {HD_HEB_SHAVUOT, N_("Shavuot/Giving of the Torah")},
  {HD_HEB_SHUSHAN_PURIM, N_("Shushan Purim")},
  {HD_HEB_SIMCHAT_TORAH, N_("Simchat Torah")},
  {HD_HEB_SUKKOT, N_("Sukkot/Feast of Tabernacles")},
  {HD_HEB_TISHA_BAV, N_("Tisha B'Av/Fasting Day")},
  {HD_HEB_TU_BSHEVAT, N_("Tu B'Shevat/New Year of Trees")},
  {HD_HEB_YOM_HASHOAH, N_("Yom Hashoah/Holocaust Memorial")},
  {HD_HEB_YOM_HAZIKARON, N_("Yom Hazikaron/Soldiers Memorial")},
  {HD_HEB_YOM_HA_AZMAUT, N_("Yom Ha Azmaut/Independence Day")},
  {HD_HEB_YOM_KIPPUR, N_("Yom Kippur/Atonement Day")},
  {HD_HEB_YOM_YERUSHALAYIM, N_("Yom Yerushalayim/Jerusalem Day")},
  {HD_HOLY_INNOCENTS_DAY, N_("Holy Innocent's Day")},
  {HD_HOLY_TRINITY, N_("Holy Trinity")},
  {HD_HURAVEE_DAY, N_("Huravee Day")},
  {HD_INDEPENDENCE_DAY, N_("Independence Day")},
  {HD_INDEPENDENCE_MOVEMENT_DAY, N_("Independence Movement Day")},
  {HD_INDEPENDENCE_PROCLAMATION, N_("Independence Proclamation")},
  {HD_INDIAN_NEW_YEARS_DAY, N_("Indian New Year's Day")},
  {HD_ISL_AGA_KHANS_BIRTHDAY, N_("Aga Khan's Birthday")},
  {HD_ISL_ASHURA, N_("Ashura'")},
  {HD_ISL_EID_AL_ADHA, N_("Eid-al-Adha")},
  {HD_ISL_EID_AL_FITR, N_("Eid-al-Fitr")},
  {HD_ISL_EID_I_MILAD_UN_NABI, N_("Eid-i-Milad-un-Nabi")},
  {HD_ISL_GHADIR, N_("Ghadir")},
  {HD_ISL_IMAMAT_DAY, N_("Imamat Day")},
  {HD_ISL_NEW_YEARS_DAY, N_("Islamic New Year's Day")},
  {HD_ISL_NUZUL_AL_QURAN, N_("Nuzul-al-Qur'an")},
  {HD_ISL_QUDS_DAY, N_("Quds Day")},
  {HD_ISL_RAMADAN, N_("Ramadan")},
  {HD_ISL_SHAB_E_BARAT, N_("Shab-e-Bara't")},
  {HD_ISL_SHAB_E_MIRAJ, N_("Shab-e-Mi'raj")},
  {HD_ISL_SHAB_E_QADR, N_("Shab-e Qadr")},
  {HD_ISL_WAQF_AL_ARAFAT, N_("Waqf-al-Arafat")},
  {HD_JAPANESE_NEW_YEARS_DAY, N_("Japanese New Year's Day")},
  {HD_JAPANESE_NEW_YEARS_EVE, N_("Japanese New Year's Eve")},
  {HD_JESUS_CIRCUMCISION, N_("Jesus' Circumcision")},
  {HD_KWANZAA, N_("Kwanzaa")},
  {HD_LABOUR_DAY, N_("Labour Day")},
  {HD_LABOUR_THANKSGIVING_DAY, N_("Labour Thanksgiving Day")},
  {HD_LAO_TZES_BIRTHDAY, N_("Lao Tze's Birthday")},
  {HD_LIBERATION_DAY, N_("Liberation Day")},
  {HD_MARINE_DAY, N_("Marine Day")},
  {HD_MARTINIMAS, N_("Martinimas")},
  {HD_MARTIN_LUTHER_KINGS_DAY, N_("Martin L. King's Day")},
  {HD_MARTYRS_DAY, N_("Martyrs' Day")},
  {HD_MARYS_ANNUNCIATION_DAY, N_("Mary's Annunciation Day")},
  {HD_MARYS_ASCENSION_DAY, N_("Mary's Ascension Day")},
  {HD_MARYS_CANDLEMAS, N_("Mary's Candlemas")},
  {HD_MARYS_EXPECTATION, N_("Mary's Expectation")},
  {HD_MARYS_IMMACULATE_CONCEPTION, N_("Mary's Immaculate Conception")},
  {HD_MARYS_MATERNITY, N_("Mary's Maternity")},
  {HD_MARYS_NAME, N_("Mary's Name")},
  {HD_MARYS_NATIVITY, N_("Mary's Nativity")},
  {HD_MARYS_SACRIFICE, N_("Mary's Sacrifice")},
  {HD_MARYS_VISITATION, N_("Mary's Visitation")},
  {HD_MARY_BLESSED_VIRGIN, N_("Mary - Blessed Virgin")},
  {HD_MAUNDY_THURSDAY, N_("Maundy Thursday")},
  {HD_MAY_DAY, N_("May Day")},
  {HD_MIDSUMMER_DAY, N_("St John's/Midsummer Day")},
  {HD_MID_AUTUMN_FESTIVAL, N_("Mid-Autumn Festival")},
  {HD_MID_SPRING_FESTIVAL, N_("Mid-Spring Festival")},
  {HD_MID_YEAR_FESTIVAL, N_("Mid-Year Festival")},
  {HD_MOON_FIRST_QUARTER, N_("Waxing Half Moon")},
  {HD_MOON_FULL, N_("Full Moon")},
  {HD_MOON_LAST_QUARTER, N_("Waning Half Moon")},
  {HD_MOON_NEW, N_("New Moon")},
  {HD_MOTHERS_DAY, N_("Mother's Day")},
  {HD_MOTHER_IN_LAWS_DAY, N_("Mother in Law's Day")},
  {HD_MUSIC_WATER_BANQUET, N_("Music Water Banquet")},
  {HD_NATIONAL_FOUNDATION_DAY, N_("National Foundation Day")},
  {HD_NATIONAL_HOLIDAY, N_("National Holiday")},
  {HD_NATIONAL_MOURNING_DAY, N_("National Mourning Day")},
  {HD_NEW_YEARS_DAY, N_("New Year's Day")},
  {HD_NURSES_DAY, N_("Nurses' Day")},
  {HD_OLD_ARMENIC_NEW_YEARS_DAY, N_("Old-Armenic New Year")},
  {HD_OLD_EGYPTIC_NEW_YEARS_DAY, N_("Old-Egyptic New Year")},
  {HD_ORTHODOX_NEW_YEARS_DAY, N_("Orthodox New Year's Day")},
  {HD_OUR_LADY_APARECIDA_DAY, N_("Our Lady Aparecida Day")},
  {HD_PALM_SUNDAY, N_("Palm Sunday")},
  {HD_PARENTS_DAY, N_("Parent's Day")},
  {HD_PASSION_SUNDAY, N_("Passion Sunday")},
  {HD_PEACE_FESTIVAL, N_("Peace Festival")},
  {HD_PEACH_FESTIVAL, N_("Peach Festival")},
  {HD_PENTECOST, N_("Whitsunday/Pentecost")},
  {HD_PERSIAN_NEW_YEARS_DAY, N_("Noruz/Persian New Year's Day")},
  {HD_PRAYER_DAY, N_("Prayer Day")},
  {HD_PRESIDENTIAL_INAUGURATION, N_("Presidential Inauguration")},
  {HD_PRESIDENTS_DAY, N_("Presidents' Day")},
  {HD_PRESIDENTS_INFORM, N_("Presidents' Inform")},
  {HD_PRESIDENT_LINCOLNS_BIRTHDAY, N_("President Lincoln's Birthday")},
  {HD_PRINCESS_DAY, N_("Princess' Day")},
  {HD_QUINQUAGESIMA_SUNDAY, N_("Quinquagesima Sunday")},
  {HD_REFORMATION_DAY, N_("Reformation Day")},
  {HD_REMEMBRANCE_DAY, N_("Remembrance/Memorial Day")},
  {HD_RE_DAY, N_("Republic Day")},
  {HD_RESPECT_FOR_THE_AGED_DAY, N_("Respect for the Aged Day")},
  {HD_REVOLUTION_DAY, N_("Revolution Day")},
  {HD_RIO_DE_JANEIRO_ANNIVERSARY, N_("Rio de Janeiro Anniversary")},
  {HD_ROGATION_SUNDAY, N_("Rogation Sunday")},
  {HD_SAO_PAULO_ANNIVERSARY, N_("Sao Paulo Anniversary")},
  {HD_SAPPORO_SNOW_FESTIVAL, N_("Sapporo Snow Festival")},
  {HD_SEASON_CHANGE, N_("Season Change/Bean-Festival")},
  {HD_SEPTUAGESIMA_SUNDAY, N_("Septuagesima Sunday")},
  {HD_SEVEN_SLEEPERS_DAY, N_("Seven Sleepers Day")},
  {HD_SEXAGESIMA_SUNDAY, N_("Sexagesima Sunday")},
  {HD_SHROVE_TUESDAY, N_("Shrove Tuesday/Mardi Gras")},
  {HD_SOLSTICE_DAY, N_("Solstice Day")},
  {HD_SOOT_SWEEPING_DAY, N_("Soot-Sweeping Day")},
  {HD_SPRING_FESTIVAL, N_("Spring Festival")},
  {HD_START_OF_COMMON_MONTH, N_("Start of common month")},
  {HD_START_OF_LEAP_MONTH, N_("Start of leap month")},
  {HD_STAR_FESTIVAL, N_("Star Festival")},
  {HD_ST_ANDREWS_DAY, N_("St Andrew's Day")},
  {HD_ST_BARTHOLOMEW_DAY, N_("St Bartholomew Day")},
  {HD_ST_BERCHTOLDS_DAY, N_("St Berchtold's Day")},
  {HD_ST_DAVIDS_DAY, N_("St David's Day")},
  {HD_ST_EDWARDS_DAY, N_("St Edward's Day")},
  {HD_ST_GEORGES_DAY, N_("St George's Day")},
  {HD_ST_JAMES_DAY, N_("St James' Day")},
  {HD_ST_JOSEPHS_DAY, N_("St Joseph's Day")},
  {HD_ST_LAURENTIUS_DAY, N_("St Laurentius Day")},
  {HD_ST_NICHOLAS_DAY, N_("St Nicholas' Day")},
  {HD_ST_NICHOLAS_EVE, N_("St Nicholas' Eve")},
  {HD_ST_PATRICKS_DAY, N_("St Patrick's Day")},
  {HD_ST_PETER_AND_ST_PAUL, N_("St Peter and St Paul")},
  {HD_ST_STEPHENS_DAY, N_("St Stephen's Day")},
  {HD_ST_VALENTINES_DAY, N_("St Valentine's Day")},
  {HD_SUNDAY_OF_THE_DEAD, N_("Sunday of the Dead")},
  {HD_SWALLOW_DAY, N_("Swallow Day")},
  {HD_SWEETEST_DAY, N_("Sweetest Day")},
  {HD_SYLVESTER, N_("Sylvester/New Year's Eve")},
  {HD_TEACHERS_DAY, N_("Teacher's Day")},
  {HD_THAI_NEW_YEARS_DAY, N_("Thai New Year's Day")},
  {HD_THANKSGIVING_DAY, N_("Thanksgiving Day")},
  {HD_THE_KINGS_BIRTHDAY, N_("The King's Birthday")},
  {HD_THE_QUEENS_BIRTHDAY, N_("The Queen's Birthday")},
  {HD_THREE_KINGS_DAY, N_("Epiphany/Three King's Day")},
  {HD_TIRADENTES_DAY, N_("Tiradentes Day")},
  {HD_TOMB_SWEEPING_DAY, N_("Tomb-Sweeping Day")},
  {HD_TRANSFIGURATION_DAY, N_("Transfiguration Day")},
  {HD_TYNWALD_DAY, N_("Tynwald Day")},
  {HD_VALBORGS_EVE, N_("Valborg's Eve")},
  {HD_VENICE_CARNIVAL, N_("Venice Carnival")},
  {HD_VETERANS_DAY, N_("Veteran's Day")},
  {HD_VICTORIA_DAY, N_("Victoria Day")},
  {HD_VICTORY_DAY, N_("Victory Day")},
  {HD_WHIT_MONDAY, N_("Whit Monday")},
  {HD_WOMENS_DAY, N_("Women's Day")},
  {HD_WOMENS_SHROVE_DAY, N_("Women's Shrove Day")},
  {HD_WORLD_ANIMAL_DAY, N_("World Animal Day")},
  {HD_YOUTH_DAY, N_("Youth Day")}
#if !HD_TOP20CC
  /*
     This COMMA is necessary here!
   */
  ,
  {HD_ADELAIDE_CUP_DAY, N_("Adelaide Cup Day")},
  {HD_ADMISSION_DAY, N_("Admission Day")},
  {HD_ALASKA_DAY, N_("Alaska Day")},
  {HD_ALEKSIS_KIVI_DAY, N_("Aleksis Kivi Day")},
  {HD_ALICE_SPRINGS_SHOW_DAY, N_("Alice-Springs Show Day")},
  {HD_ALL_SAINTS_EVE, N_("All Saints' Eve")},
  {HD_AMERICAS_DAY, N_("Americas Day")},
  {HD_ANGAM_DAY, N_("Angam Day")},
  {HD_ANNIVERSARY_OF_AMIRS_SUCCESSION,
   N_("Anniversary of Amir's Succession")},
  {HD_ANNIVERSARY_OF_ARMED_STRUGGLE, N_("Anniversary of Armed Struggle")},
  {HD_ANNIVERSARY_OF_GREEN_MARCH, N_("Anniversary of Green March")},
  {HD_ANTILLIAN_DAY, N_("Antillian Day")},
  {HD_ARAB_LEAGUE_DAY, N_("Arab League Day")},
  {HD_ARCHBISHOP_MAKARIOS_MEMORIAL_DAY,
   N_("Archbishop Makarios' Memorial Day")},
  {HD_ARCHBISHOP_MAKARIOS_NAME_DAY, N_("Archbishop Makarios' Name-Day")},
  {HD_ARENGO_ANNIVERSARY, N_("Arengo Anniversary")},
  {HD_ARMY_COUP_DAY, N_("Army Coup Day")},
  {HD_ARRIVAL_OF_THE_SWISS, N_("Arrival of the Swiss")},
  {HD_ARTIGAS_BIRTHDAY, N_("Artigas' Birthday")},
  {HD_ASUNCION_DAY, N_("Asuncion Day")},
  {HD_ATATURK_MEMORIAL_DAY, N_("Ataturk Memorial Day")},
  {HD_AUTHORITIYS_POWER_DAY, N_("Authority's Power Day")},
  {HD_BAATH_REVOLUTION_DAY, N_("Ba'ath Revolution Day")},
  {HD_BARON_BLISS_DAY, N_("Baron Bliss Day")},
  {HD_BATAAN_DAY, N_("Bataan Day")},
  {HD_BATTLE_OF_ANGAMOS, N_("Battle of Angamos")},
  {HD_BATTLE_OF_BOYACA, N_("Battle of Boyacá")},
  {HD_BATTLE_OF_CARABOBO, N_("Battle of Carabobo")},
  {HD_BATTLE_OF_IQUIQUE, N_("Battle of Iquique")},
  {HD_BATTLE_OF_LAS_PIEDRAS, N_("Battle of Las Piedras")},
  {HD_BATTLE_OF_NAEFELS, N_("Battle of Näfels")},
  {HD_BEGINNING_OF_SUMMER, N_("Beginning of Summer")},
  {HD_BELGRADES_DAY, N_("Belgrade's Day")},
  {HD_BENNINGTON_BATTLE_DAY, N_("Bennington Battle Day")},
  {HD_BLESSING_OF_THE_WATER, N_("Blessing of the Water")},
  {HD_BLOOMSDAY, N_("Bloomsday")},
  {HD_BOGANDA_DAY, N_("Boganda Day")},
  {HD_BONAIRE_DAY, N_("Bonaire Day")},
  {HD_BOQUERON_BATTLE_DAY, N_("Boqueron Battle Day")},
  {HD_BOTSWANA_DAY, N_("Botswana Day")},
  {HD_BOUNTY_DAY, N_("Bounty Day")},
  {HD_BUNKER_HILL_DAY, N_("Bunker Hill Day")},
  {HD_BURNING_OF_JAN_HUS, N_("Burning of Jan Hus")},
  {HD_CANBERRA_DAY, N_("Canberra Day")},
  {HD_CARICOM_DAY, N_("Caricom Day")},
  {HD_CASIMIR_PULASKIS_BIRTHDAY, N_("Casimir Pulaski's Birthday")},
  {HD_CASSINGA_DAY, N_("Cassinga Day")},
  {HD_CAYENNE_FESTIVAL, N_("Cayenne Festival")},
  {HD_CESAR_CHAVEZ_DAY, N_("Cesar Chavez Day")},
  {HD_CHARTER_DAY, N_("Charter Day")},
  {HD_CHIANG_KAI_SHEKS_BIRTHDAY, N_("Chiang Kai-shek's Birthday")},
  {HD_CHILDRENS_WHITE_SUNDAY, N_("Children's White Sunday")},
  {HD_COLON_DAY, N_("Colon Day")},
  {HD_COLORADO_DAY, N_("Colorado Day")},
  {HD_COMMONWEALTH_DAY, N_("Commonwealth Day")},
  {HD_COMPACT_DAY, N_("Compact Day")},
  {HD_CONFEDERAL_AGREEMENT_DAY, N_("Confederal Agreement Day")},
  {HD_CORONATION_DAY, N_("Coronation Day")},
  {HD_CUP_MATCH_DAY, N_("Cup Match Day")},
  {HD_CUSTOM_CHIEFS_DAY, N_("Custom Chief's Day")},
  {HD_C_MAPINDUZI_DAY, N_("C. Mapinduzi Day")},
  {HD_DARWIN_SHOW_DAY, N_("Darwin Show Day")},
  {HD_DAY_OF_GOODWILL, N_("Day of Goodwill")},
  {HD_DAY_OF_STUDENTS_REVOLT, N_("Day of Students Revolt")},
  {HD_DAY_OF_THE_FREE_LAOS, N_("Day of the Free Laos")},
  {HD_DEATH_OF_H_CHRISTOPHE, N_("Death of H. Christophe")},
  {HD_DEATH_OF_J_DESSALINES, N_("Death of J. Dessalines")},
  {HD_DEATH_OF_PRESIDENT_ABDALLAH, N_("Death of President Abdallah")},
  {HD_DEATH_OF_PRESIDENT_CHEIKH, N_("Death of President Cheikh")},
  {HD_DEATH_OF_PRESIDENT_SOILIH, N_("Death of President Soilih")},
  {HD_DEATH_OF_QAID_I_AZAM, N_("Death of Qaid-i-Azam")},
  {HD_DEATH_OF_T_LOUVERTURE, N_("Death of T. Louverture")},
  {HD_DEFENDERS_DAY, N_("Defenders Day")},
  {HD_DIA_DEL_PADRE, N_("Dia del Padre")},
  {HD_DIA_DE_LOS_MAESTROS, N_("Dia de los Maestros")},
  {HD_DISCOVERY_DAY, N_("Discovery Day")},
  {HD_DOWNFALL_OF_THE_DERGUE, N_("Downfall of the Dergue")},
  {HD_DRY_SEASON_CELEBRATION, N_("Dry Season Celebration")},
  {HD_DR_SUN_YAT_SENS_BIRTHDAY, N_("Dr. Sun Yat-sen's Birthday")},
  {HD_DUARTES_BIRTHDAY, N_("Duarte's Birthday")},
  {HD_EASTER_TUESDAY, N_("Easter Tuesday")},
  {HD_ECONOMIC_LIBERATION_DAY, N_("Economic Liberation Day")},
  {HD_EIGHT_HOURS_DAY, N_("Eight Hours Day")},
  {HD_ELECTIONS_FOR_NATIONAL_ASSEMBLY, N_("Elections for National Assembly")},
  {HD_ERROL_BARROW_DAY, N_("Errol Barrow Day")},
  {HD_EVACUATION_DAY, N_("Evacuation Day")},
  {HD_EVE_OF_EPIPHANY, N_("Eve of Epiphany")},
  {HD_EXPEDITION_OF_THE_33, N_("Expedition of the 33")},
  {HD_E_HOSTOS_BIRTHDAY, N_("E. Hostos' Birthday")},
  {HD_FAMILY_DAY, N_("Family Day")},
  {HD_FAO_DAY, N_("FAO Day")},
  {HD_FARMERS_DAY, N_("Farmers' Day")},
  {HD_FATHER_LEVAL_DAY, N_("Father Leval Day")},
  {HD_FEAST_OF_OUR_THEOTOKOS, N_("Feast of Our Theotokos")},
  {HD_FISHERMANS_DAY, N_("Fisherman's Day")},
  {HD_FOUNDATION_OF_NPLA_DAY, N_("Foundation of NPLA Day")},
  {HD_FOUNDING_OF_PEOPLES_PARTY, N_("Founding of People's Party")},
  {HD_GARIFUNA_DAY, N_("Garifuna Day")},
  {HD_GENERAL_SAN_MARTINS_ANNIVERSARY,
   N_("General San Martin's Anniversary")},
  {HD_GOSPEL_DAY, N_("Gospel Day")},
  {HD_GUACANASTE_DAY, N_("Guacanaste Day")},
  {HD_GUSTAVUS_ADOLPHUS_DAY, N_("Gustavus Adolphus' Day")},
  {HD_HERITAGE_DAY, N_("Heritage Day")},
  {HD_HEROES_DAY, N_("Heroes Day")},
  {HD_HOBART_SHOW_DAY, N_("Hobart Show Day")},
  {HD_HOLY_WEDNESDAY, N_("Holy Wednesday")},
  {HD_HO_CHI_MINHS_BIRTHDAY, N_("Ho Chi Minh's Birthday")},
  {HD_HUMAN_RIGHTS_DAY, N_("Human Rights Day")},
  {HD_HURRICANE_SUPPLICATION_DAY, N_("Hurricane Supplication Day")},
  {HD_HURRICANE_THANKSGIVING_DAY, N_("Hurricane Thanksgiving Day")},
  {HD_INDEPENDENCE_OF_CARTAGENA, N_("Independence of Cartagena")},
  {HD_INDEPENDENCE_OF_CUENCA, N_("Independence of Cuenca")},
  {HD_INDEPENDENCE_OF_GUAYAQUIL, N_("Independence of Guayaquil")},
  {HD_INDEPENDENCE_OF_QUITO, N_("Independence of Quito")},
  {HD_INDIAN_ARRIVAL_DAY, N_("Indian Arrival Day")},
  {HD_INTERNAL_AUTONOMY_DAY, N_("Internal Autonomy Day")},
  {HD_JEFFERSON_DAVIS_BIRTHDAY, N_("Jefferson Davis' Birthday")},
  {HD_JUNE_HOLIDAY, N_("June Holiday")},
  {HD_J_BARBOSAS_BIRTHDAY, N_("J. Barbosa's Birthday")},
  {HD_J_CHILEMBWE_DAY, N_("J. Chilembwe Day")},
  {HD_J_DIEGOS_BIRTHDAY, N_("J. Diego's Birthday")},
  {HD_J_ROBERTS_BIRTHDAY, N_("J. Robert's Birthday")},
  {HD_KAMARAMPAKA_DAY, N_("Kamarampaka Day")},
  {HD_KARTINI_DAY, N_("Kartini Day")},
  {HD_KATHERINE_SHOW_DAY, N_("Catherine Show Day")},
  {HD_KIEV_DAY, N_("Kiev Day")},
  {HD_KIM_IL_SUNGS_BIRTHDAY, N_("Kim Il-Sung's Birthday")},
  {HD_KIM_JONG_ILS_BIRTHDAY, N_("Kim Jong-Il's Birthday")},
  {HD_KING_KAMEHAMEHA_DAY, N_("King Kamehameha Day")},
  {HD_KING_RAMA_I_MEMORIAL_DAY, N_("King Rama I Memorial Day/Chakri")},
  {HD_KING_RAMA_V_MEMORIAL_DAY, N_("King Rama V Memorial Day")},
  {HD_KNABENSCHIESSEN, N_("Knabenschiessen")},
  {HD_KONSTANTIN_AND_METHODIUS, N_("Konstantin and Methodius")},
  {HD_LAPP_NATIONAL_HOLIDAY, N_("Lapp National Holiday")},
  {HD_LAUNCESTON_CUP_DAY, N_("Launceston Cup Day")},
  {HD_LIBERTY_DAY, N_("Liberty Day")},
  {HD_LOSS_OF_MUSLIM_NATION, N_("Loss of Muslim Nation")},
  {HD_LUXEMBOURG_CITY_KERMIS, N_("Luxembourg City Kermis")},
  {HD_LYNDON_B_JOHNSON_DAY, N_("Lyndon B. Johnson Day")},
  {HD_L_RIVERAS_BIRTHDAY, N_("L. Rivera's Birthday")},
  {HD_MADARAKA_DAY, N_("Madaraka Day")},
  {HD_MAHATMA_GANDHIS_BIRTHDAY, N_("Mahatma Gandhi's Birthday")},
  {HD_MALVINAS_DAY, N_("Malvinas Day")},
  {HD_MANILA_DAY, N_("Manila Day")},
  {HD_MAPUTO_CITY_DAY, N_("Maputo City Day")},
  {HD_MARIEN_NGOUABI_DAY, N_("Marien Ngouabi Day")},
  {HD_MARYLAND_DAY, N_("Maryland Day")},
  {HD_MELBOURNE_CUP_DAY, N_("Melbourne Cup Day")},
  {HD_MERCHANT_DAY, N_("Merchant Day")},
  {HD_MIDSUMMERS_EVE, N_("St John's/Midsummers Eve")},
  {HD_MISSIONARY_DAY, N_("Missionary Day")},
  {HD_MI_CAREME_DAY, N_("Mi-Carême Day")},
  {HD_MORAZAN_DAY, N_("Morazan Day")},
  {HD_MOSHESHOES_DAY, N_("Mosheshoe's Day")},
  {HD_MOTHERHOOD_AND_BEAUTY_DAY, N_("Motherhood and Beauty Day")},
  {HD_NAMING_DAY, N_("Naming Day")},
  {HD_NATIONAL_BUN_DAY, N_("National Bun Day")},
  {HD_NATIONAL_HEROES_DAY, N_("National Heroes Day")},
  {HD_NATIONAL_REDEMPTION_DAY, N_("National Redemption Day")},
  {HD_NATIONAL_RESISTANCE_DAY, N_("National Resistance Day")},
  {HD_NATIONAL_REVIVAL_DAY, N_("National Revival Day")},
  {HD_NATIONAL_UNITY_DAY, N_("National Unity Day")},
  {HD_NDADAYE_DAY, N_("Ndadaye Day")},
  {HD_NEUTRALITY_DAY, N_("Neutrality Day")},
  {HD_NEVADA_DAY, N_("Nevada Day")},
  {HD_NEW_REGIME_ANNIVERSARY, N_("New Regime Anniversary")},
  {HD_NOBEL_DAY, N_("Nobel Day")},
  {HD_NOI_DAY, N_("Noi Day")},
  {HD_OAU_DAY, N_("OAU Day")},
  {HD_OCTOBER_HOLIDAY, N_("October Holiday")},
  {HD_OIL_INDUSTRY_DAY, N_("Oil Industry Day")},
  {HD_OUR_LADY_OF_ALTAGRACIA_DAY, N_("Our Lady of Altagracia Day")},
  {HD_OUR_LADY_OF_CAMARIN_DAY, N_("Our Lady of Camarin Day")},
  {HD_OUR_LADY_OF_LAS_MERCEDES_DAY, N_("Our Lady of Las Mercedes Day")},
  {HD_OUR_LADY_OF_LOS_ANGELES_DAY, N_("Our Lady of Los Angeles Day")},
  {HD_OUR_LADY_OF_SEVEN_SORROWS, N_("Our Lady of Seven Sorrows")},
  {HD_OUR_LADY_OF_VICTORIES_DAY, N_("Our Lady of Victories Day")},
  {HD_PANAMA_CITY_DAY, N_("Panama City Day")},
  {HD_PANAMERICAN_DAY, N_("Panamerica Day")},
  {HD_PARIS_PEACE_AGREEMENT_DAY, N_("Paris Peace Agreement Day")},
  {HD_PATRIOTS_DAY, N_("Patriot's Day")},
  {HD_PEOPLES_UPRISING_DAY, N_("People's Uprising Day")},
  {HD_PEOPLE_POWER_DAY, N_("People Power Day")},
  {HD_PHILIPPINE_USA_FRIENDSHIP, N_("Philippine-American Friendship")},
  {HD_PICHINCHA_DAY, N_("Pichincha Day")},
  {HD_PICNIC_DAY, N_("Picnic Day")},
  {HD_PIONEER_DAY, N_("Pioneer Day")},
  {HD_POYA_DAY, N_("Poya Day")},
  {HD_PRINCE_KALANIANAOLE_DAY, N_("Prince Kalanianaole Day")},
  {HD_PRINCE_OF_WALES_BIRTHDAY, N_("Prince of Wales' Birthday")},
  {HD_PROCLAMATION_DAY, N_("Proclamation Day")},
  {HD_QAID_I_AZAMS_BIRTHDAY, N_("Qaid-i-Azam's Birthday")},
  {HD_RATU_SUKUMA_DAY, N_("Ratu Sukuma Day")},
  {HD_RECREATION_DAY, N_("Recreation Day")},
  {HD_REFERENDUM_DAY, N_("Referendum Day")},
  {HD_REGATTA_DAY, N_("Regatta Day")},
  {HD_REGENCY_EXCHANGE, N_("Regency Exchange")},
  {HD_RESTORATION_DAY, N_("Restoration Day")},
  {HD_RETURN_DAY, N_("Return Day")},
  {HD_REUNIFICATION_DAY, N_("Reunification Day")},
  {HD_RIZAL_DAY, N_("Rizal Day")},
  {HD_RWAGASORE_DAY, N_("Rwagasore Day")},
  {HD_SABA_DAY, N_("Saba Day")},
  {HD_SAN_JACINTO_DAY, N_("San Jacinto Day")},
  {HD_SAN_JOSE_DAY, N_("San José Day")},
  {HD_SAR_ESTABLISHMENT_DAY, N_("SAR Establishment Day")},
  {HD_SCHOOL_HOLIDAY, N_("School Holiday")},
  {HD_SECHSELAEUTEN, N_("Sechseläuten")},
  {HD_SEPARATION_DAY, N_("Separation Day")},
  {HD_SERETSE_KHAMA_DAY, N_("Seretse Khama Day")},
  {HD_SETTLERS_DAY, N_("Settlers Day")},
  {HD_SEWARDS_DAY, N_("Seward's Day")},
  {HD_SHAHEED_DAY, N_("Shaheed Day")},
  {HD_SHEEP_FESTIVAL, N_("Sheep Festival")},
  {HD_SHEIKH_ZAYEDS_ASCENSION_DAY, N_("Sheikh Zayed's Ascension Day")},
  {HD_SIMON_BOLIVAR_DAY, N_("Simon Bolivar's Day")},
  {HD_SINAI_LIBERATION_DAY, N_("Sinai-Liberation Day")},
  {HD_SLAVE_LIBERATION_DAY, N_("Slave-Liberation Day")},
  {HD_SNIFF_THE_BREEZE_DAY, N_("Sniff the Breeze Day")},
  {HD_SOLIDARITY_DAY, N_("Solidarity Day")},
  {HD_SOMERS_DAY, N_("Somers Day")},
  {HD_STATIA_AMERICA_DAY, N_("Statia-American Day")},
  {HD_STUDENTS_DAY, N_("Student's Day")},
  {HD_ST_AGATA_DAY, N_("St Agata Day")},
  {HD_ST_CANUTES_DAY, N_("St Canute's Day")},
  {HD_ST_CEDILIAS_DAY, N_("St Cedilia's Day")},
  {HD_ST_CHARLES_DAY, N_("St Charles' Day")},
  {HD_ST_DEMETRIUS_DAY, N_("St Demetrius' Day")},
  {HD_ST_DEVOTE_DAY, N_("St Dévote Day")},
  {HD_ST_ELIAHS_DAY, N_("St Eliah's Day")},
  {HD_ST_JEAN_BAPTISTE_DAY, N_("St Jean Baptiste Day")},
  {HD_ST_LUCIAS_DAY, N_("St Lucia's Day")},
  {HD_ST_MARGUERITES_DAY, N_("St Marguerite's Day")},
  {HD_ST_MAROONS_DAY, N_("St Maroon's Day")},
  {HD_ST_MICHAELS_DAY, N_("St Michael's Day")},
  {HD_ST_OLAVS_DAY, N_("St Olav's Day")},
  {HD_ST_OLAVS_EVE, N_("St Olav's Eve")},
  {HD_ST_PAULS_SHIPSWRECK, N_("St Paul's Shipswreck")},
  {HD_ST_ROSE_OF_LIMA, N_("St Rose of Lima")},
  {HD_ST_URSULAS_DAY, N_("St Ursula's Day")},
  {HD_ST_VINCENT_DE_PAULS_DAY, N_("St Vincent de Paul's Day")},
  {HD_SUEZ_VICTORY_DAY, N_("Suez Victory Day")},
  {HD_SULTAN_QABOOS_BIRTHDAY, N_("Sultan Qaboos' Birthday")},
  {HD_SVETITSKHOVLOBA, N_("Svetitskhovloba")},
  {HD_SWEDEN_DAY, N_("Sweden Day")},
  {HD_S_DOES_BIRTHDAY, N_("S. Doe's Birthday")},
  {HD_TENNANT_CREEK_SHOW_DAY, N_("Tennant-Creek Show Day")},
  {HD_TERRITORY_DAY, N_("Territory Day")},
  {HD_THE_CROWN_PRINCESSES_BIRTHDAY, N_("The Crown Princesse's Birthday")},
  {HD_THE_CROWN_PRINCESSES_NAME_DAY, N_("The Crown Princesse's Name-Day")},
  {HD_THE_CROWN_PRINCES_BIRTHDAY, N_("The Crown Prince's Birthday")},
  {HD_THE_KINGS_NAME_DAY, N_("The King's Name-Day")},
  {HD_THE_QUEENS_NAME_DAY, N_("The Queen's Name-Day")},
  {HD_TOPOU_I_DAY, N_("Topou I Day")},
  {HD_TOWN_MEETING_DAY, N_("Town Meeting Day")},
  {HD_TRADITIONAL_DAY, N_("Traditional Day")},
  {HD_TRANSFER_DAY, N_("Transfer Day")},
  {HD_TRUMAN_DAY, N_("Truman Day")},
  {HD_UNION_DAY, N_("Union Day")},
  {HD_UNION_WITH_SWEDEN_DISSOLVED, N_("Union with Sweden dissolved")},
  {HD_UNITED_NATIONS_DAY, N_("United Nations Day")},
  {HD_UNITY_FACTORY_DAY, N_("Unity Factory Day")},
  {HD_VICTOR_SCHOELCHER_DAY, N_("Victor-Schoelcher Day")},
  {HD_VIDOVDAN, N_("Vidovdan")},
  {HD_WEST_VIRGINIA_DAY, N_("West Virginia Day")},
  {HD_WHITSUN_EVE, N_("Whitsun Eve")},
  {HD_W_TUBMANS_BIRTHDAY, N_("W. Tubman's Birthday")},
  {HD_YAP_DAY, N_("Yap Day")},
  {HD_DAY_OF_CZECH_STATEHOOD, N_("Day of Czech Statehood")}
#endif	/* !HD_TOP20CC */
};



/*
*  static functions prototypes.
*/
__BEGIN_DECLARATIONS
/*
************************************************** Defined in `hd-data.c'.
*/
static char *decode_eclipse __P_ ((double *eclipse));
static void
  ocs_monthtext __P_ ((Bool * init_data,
		       const Bool detected,
		       const char *ptr_cc_id,
		       const int day,
		       const int month,
		       const int year,
		       const Bool ocs_leap,
		       const int ocs_month,
		       const int ocs_year,
		       const int ocs_extra,
		       int *hd_elems, const int fday, const int count));
__END_DECLARATIONS
/*
*  static variables definitions.
*/
/*! Number of past days of Hebrew month. */
static const int hmvec[][MONTH_MAX + 1] = { {0, 30, 59, 88, 117, 147,
					     147, 176, 206, 235, 265, 294,
					     324},
{0, 30, 59, 89, 118, 148,
 148, 177, 207, 236, 266, 295, 325},
{0, 30, 60, 90, 119, 149,
 149, 178, 208, 237, 267, 296, 326},
{0, 30, 59, 88, 117, 147,
 177, 206, 236, 265, 295, 324, 354},
{0, 30, 59, 89, 118, 148,
 178, 207, 237, 266, 296, 325, 355},
{0, 30, 60, 90, 119, 149,
 179, 208, 238, 267, 297, 326, 356}
};

/*! Latin Zodiac names. */
static const char *zod_name[] = {
  "Aries", "Taurus", "Gemini",
  "Cancer", "Leo", "Virgo",
  "Libra", "Scorpius", "Sagittarius",
  "Capricornus", "Aquarius", "Pisces"
};

/*! Chinese (Mandarin??) names for the celestial stems. */
static const char *chi_stem[] = {
  "Jia", "Yi",
  "Bing", "Ding",
  "Wu", "Ji",
  "Geng", "Xin",
  "Ren", "Gui"
};

/*! Chinese (Mandarin??) names for the heavenly branches. */
static const char *chi_branch[] = {
  N_("Zi/Rat"), N_("Chou/Ox"),
  N_("Yin/Tiger"), N_("Mao/Rabbit"),
  N_("Chen/Dragon"), N_("Si/Snake"),
  N_("Wu/Horse"), N_("Wei/Sheep"),
  N_("Shen/Monkey"), N_("You/Rooster"),
  N_("Xu/Dog"), N_("Hai/Pig")
};



/*
*  Function implementations.
*/
static char *
decode_eclipse (eclipse)
     double *eclipse;
/*!
   Decodes the eclipse mode value given via the address of ECLIPSE
     in that it is reduced to the Julian/Gregorian date with time fraction
     and returns a pointer to the proper eclipse mode text found.
*/
{
  if (*eclipse > ECLIPSE_TOTAL)
    {
      *eclipse -= ECLIPSE_TOTAL;
      return (_(hd_text[HD_ECLIPSE_TYPE_TOTAL].ht_text));
    }
  if (*eclipse > ECLIPSE_ANNULAR)
    {
      *eclipse -= ECLIPSE_ANNULAR;
      return (_(hd_text[HD_ECLIPSE_TYPE_ANNULAR].ht_text));
    }
  if (*eclipse > ECLIPSE_PARTIAL)
    {
      *eclipse -= ECLIPSE_PARTIAL;
      return (_(hd_text[HD_ECLIPSE_TYPE_PARTIAL].ht_text));
    }
  if (*eclipse > ECLIPSE_PENUMBRAL)
    {
      *eclipse -= ECLIPSE_PENUMBRAL;
      return (_(hd_text[HD_ECLIPSE_TYPE_PENUMBRAL].ht_text));
    }
  /*
     Internal error, misplaced eclipse mode constants defined by maintainer!
   */
  abort ();

  /*
     Notreached, only used to make the compiler happy :)
   */
  return (NULL);
}



static void
ocs_monthtext (init_data, detected, ptr_cc_id, day, month, year,
	       ocs_leap, ocs_month, ocs_year, ocs_extra, hd_elems, fday,
	       count)
     Bool *init_data;
     const Bool detected;
     const char *ptr_cc_id;
     const int day;
     const int month;
     const int year;
     const Bool ocs_leap;
     const int ocs_month;
     const int ocs_year;
     const int ocs_extra;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Formats a text that includes the given OCS_MONTH and OCS_YEAR number,
     which is based on another calendar systems date.  The given PTR_CC_ID
     is also placed in the text, provided with a trailing SUFFIX character.
*/
{
  static char the_cc_id[7];
  auto char *the_text;


  strcpy (the_cc_id, ptr_cc_id);
  strcat (the_cc_id, "*");
  if (ocs_leap)
    the_text = _(hd_text[HD_START_OF_LEAP_MONTH].ht_text);
  else
    the_text = _(hd_text[HD_START_OF_COMMON_MONTH].ht_text);
  if (ocs_extra != SPECIAL_VALUE)
    sprintf (s2, "%s %02d/%d-%d", the_text, ocs_month, ocs_year, ocs_extra);
  else
    sprintf (s2, "%s %02d/%d", the_text, ocs_month, ocs_year);
  holiday (*init_data, detected, s2, the_cc_id,
	   DIS_HLS_PREF, day, month, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
}



const Cc_struct *
binsearch_cc_id (id)
     char *id;
/*!
   Simple binary search the delivered `id' if it is listed in the global
     `cc_holidays' table.  The `id' is an ISO-3166 A2 (2-letter) country
     code, plus an optionally trailing territory text `_TT'.  If `id' is
     found in the table, this function returns a pointer to its complete
     record, otherwise NULL.
   *** This functions must be placed in the same `.c' file
   *** in which the `cc_holidays' table is defined!
*/
{
  auto const Cc_struct *ptr_cc;
  register int i;
  register int low = 0;
  register int mid;
  register int high = (sizeof cc_holidays / sizeof (Cc_struct)) - 2;


  while (low <= high)
    {
      mid = ((low + high) >> 1);
      ptr_cc = cc_holidays + mid;
      i = strcmp (id, ptr_cc->cc_id);
      if (i < 0)
	high = mid - 1;
      else if (i > 0)
	low = mid + 1;
      else
	return (ptr_cc);
    }

  return (NULL);
}



void
base_christian_hdy (init_data, detected, easter, year, hd_elems, fday, count, cc)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
     const char *cc;
/*!
   Manages all base Christian Western churches calendar based holidays.
*/
{
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_PENTECOST].ht_text),
	   ptr_cc_id, "+", easter + 49, 0, year, hd_elems, fday, count);
  if (strcmp (cc, "SE") != 0)
    holiday (*init_data, detected, _(hd_text[HD_WHIT_MONDAY].ht_text),
	     ptr_cc_id, "+", easter + 50, 0, year, hd_elems, fday, count);
}



/*
*  All global holiday management functions.
*/
void
astronomical_hdy (init_data, detected, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all Astronomically based holidays (equinoxes, solstices,
   Moon phases, solar and lunar eclipses).
     Calculations are done for a line at a definite meridian expressed as a
     time value.  This time value is given by the global `time_hour_offset'
     variable and by the global `time_min_offset' variable.  If HOUR and MIN
     are set to zero, calculations are made for Universal Time (UTC/GMT).  If
     HOUR and MIN have a positive sign, UTC/GMT calculations are made for
     meridians East of Greenwich, otherwise for meridians West of Greenwich.
*/
{
  auto double mjd;
  auto double eclipse;
  auto double x;
  auto Ulint lunation;
  auto int the_min;
  auto int the_hour;
  auto int d;
  auto int m;
  auto int y = year;
  auto char *ptr_txt;
  auto char *ptr2_txt = (char *) NULL;


  ptr_cc_id = _("Ast");
  /*
     Detect all equinoxes and solstices of the YEAR.
   */
  for (x = NH_VE; x <= NH_WS; x += 90.0)
    {
      mjd =
	equinox_solstice (x, &d, &m, &y, time_hour_offset, time_min_offset);
      if (mjd != SPECIAL_VALUE)
	{
	  val2hours (mjd, &the_hour, &the_min, NULL);
	  if (x == NH_VE || x == NH_AE)
	    ptr_txt = _(hd_text[HD_EQUINOX_DAY].ht_text);
	  else
	    ptr_txt = _(hd_text[HD_SOLSTICE_DAY].ht_text);
	  sprintf (s2, "%s %02d%s%02d", ptr_txt, the_hour, time_sep, the_min);
	  holiday (*init_data, detected, s2, ptr_cc_id,
		   DIS_HLS_PREF, d, m, y, hd_elems, fday, count);
	  if (*init_data)
	    *init_data = FALSE;
	}
      else
	y = year;
    }
  /*
     Detect all New, First Quarter, Full and Last Quarter Moon phases,
     and all solar and lunar eclipses of the YEAR.
   */
  for (x = MPHASE_NEW; x <= MPHASE_LQT; x += 0.25)
    {
      lunation = 0L;
      d = DAY_MIN;
      m = MONTH_MIN;
      y = year;
      while (y == year)
	{
	  mjd = moonphase (x, TRUE, &eclipse, &lunation,
			   &d, &m, &y, time_hour_offset, time_min_offset);
	  if (y == year)
	    {
	      /*
	         Manage Moon phase.
	       */
	      switch ((int) ROUND (x * 4.0))
		{
		case 0:
		  ptr_txt = _(hd_text[HD_MOON_NEW].ht_text);
		  ptr2_txt = _(hd_text[HD_ECLIPSE_SOLAR].ht_text);
		  break;
		case 1:
		  ptr_txt = _(hd_text[HD_MOON_FIRST_QUARTER].ht_text);
		  break;
		case 2:
		  ptr_txt = _(hd_text[HD_MOON_FULL].ht_text);
		  ptr2_txt = _(hd_text[HD_ECLIPSE_LUNAR].ht_text);
		  break;
		case 3:
		  ptr_txt = _(hd_text[HD_MOON_LAST_QUARTER].ht_text);
		  break;
		default:
		  abort ();	/* Error, invalid case! */
		}
	      val2hours (mjd, &the_hour, &the_min, NULL);
	      sprintf (s2, "%s %02d%s%02d", ptr_txt, the_hour, time_sep,
		       the_min);
	      holiday (*init_data, detected, s2, ptr_cc_id, DIS_HLS_PREF, d,
		       m, year, hd_elems, fday, count);
	      /*
	         Manage eclipse.
	       */
	      if (eclipse != SPECIAL_VALUE)
		{
		  ptr_txt = decode_eclipse (&eclipse);
		  num2date ((Ulint) eclipse, &d, &m, &y);
		  val2hours (eclipse, &the_hour, &the_min, NULL);
		  sprintf (s2, "%s/%s %02d%s%02d",
			   ptr2_txt, ptr_txt, the_hour, time_sep, the_min);
		  holiday (*init_data, detected, s2, ptr_cc_id,
			   DIS_HLS_PREF, d, m, year, hd_elems, fday, count);
		}
	      lunation++;
	    }
	}
    }
}



void
bahai_hdy (init_data, detected, year, hd_elems, fday, count, cc)
     Bool *init_data;
     const Bool detected;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
     const char *cc;
/*!
   Manages all Bah'a'i calendar based holidays since AD 21-Mar-1844.
*/
{
  if (year > 1843)
    {
      register int doy;
      register int cc_mode;


      if (cc == (char *) NULL)
	cc_mode = 0;
      else if (!strcmp (cc, HD_MCNY))
	cc_mode = SPECIAL_VALUE;
      else
	abort ();		/* Error, invalid case! */
      ptr_cc_id = _("Bah");
      doy = day_of_year (21, 3, year);
      if (year <= greg->year)
	doy -= 12;
      /*
         Calculate the year number used in the Bah'a'i calendar.
       */
      sprintf (s2, "%s %d", _(hd_text[HD_BAHAI_NEW_YEARS_DAY].ht_text),
	       year - 1843);
      holiday (*init_data, detected, s2, ptr_cc_id, DIS_HLS_PREF, doy, 0,
	       year, hd_elems, fday, count);
      if (*init_data)
	*init_data = FALSE;
      if (!cc_mode)
	{
	  holiday (*init_data, detected,
		   _(hd_text[HD_BAHAI_FIRST_DAY_OF_RIDVAN].ht_text),
		   ptr_cc_id, DIS_HLS_PREF, doy + 31, 0, year, hd_elems, fday,
		   count);
	  holiday (*init_data, detected,
		   _(hd_text[HD_BAHAI_NINTH_DAY_OF_RIDVAN].ht_text),
		   ptr_cc_id, DIS_HLS_PREF, doy + 39, 0, year, hd_elems, fday,
		   count);
	  holiday (*init_data, detected,
		   _(hd_text[HD_BAHAI_TWELFTH_DAY_OF_RIDVAN].ht_text),
		   ptr_cc_id, DIS_HLS_PREF, doy + 42, 0, year, hd_elems, fday,
		   count);
	  holiday (*init_data, detected,
		   _(hd_text[HD_BAHAI_DECLARATION_OF_THE_BAB].ht_text),
		   ptr_cc_id, DIS_HLS_PREF, doy + 63, 0, year, hd_elems, fday,
		   count);
	  holiday (*init_data, detected,
		   _(hd_text[HD_BAHAI_ASCENSION_OF_BAHA_ULLAH].ht_text),
		   ptr_cc_id, DIS_HLS_PREF, doy + 69, 0, year, hd_elems, fday,
		   count);
	  holiday (*init_data, detected,
		   _(hd_text[HD_BAHAI_MARTYRDOM_OF_THE_BAB].ht_text),
		   ptr_cc_id, DIS_HLS_PREF, doy + 110, 0, year, hd_elems,
		   fday, count);
	  holiday (*init_data, detected,
		   _(hd_text[HD_BAHAI_BIRTH_OF_THE_BAB].ht_text), ptr_cc_id,
		   DIS_HLS_PREF, doy + 213, 0, year, hd_elems, fday, count);
	  holiday (*init_data, detected,
		   _(hd_text[HD_BAHAI_BIRTH_OF_BAHA_ULLAH].ht_text),
		   ptr_cc_id, DIS_HLS_PREF, doy + 236, 0, year, hd_elems,
		   fday, count);
	  holiday (*init_data, detected,
		   _(hd_text[HD_BAHAI_DAY_OF_THE_COVENANT].ht_text),
		   ptr_cc_id, DIS_HLS_PREF, doy + 250, 0, year, hd_elems,
		   fday, count);
	  holiday (*init_data, detected,
		   _(hd_text[HD_BAHAI_ASCENSION_OF_ABDUL_BAHA].ht_text),
		   ptr_cc_id, DIS_HLS_PREF, doy + 252, 0, year, hd_elems,
		   fday, count);
	}
    }
}



void
bahai_mth (init_data, detected, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all Bah'a'i calendar based months since AD 21-Mar-1844.
*/
{
  if (year > 1843)
    {
      auto Ulint bdate;
      register int bm;
      register int i;
      register int n;
      auto int d;
      auto int m;
      auto int y;


      ptr_cc_id = _("Bah");
      for (n = year - 1; n <= year; n++)
	if (n > 1843)
	  {
	    bdate = date2num (21, 3, n);
	    if (n <= greg->year)
	      bdate -= 12;
	    /*
	       And manage all Bah'a'i calendar months.
	     */
	    for (bm = MONTH_MIN, i = 0; i <= 19; bm++, i++)
	      {
		num2date (bdate, &d, &m, &y);
		if (y == year)
		  {
		    if (i == 18)
		      {
			bm--;
			bdate -= (15 - (days_of_february (n) != 29));
			ocs_monthtext (init_data, detected, ptr_cc_id, d, m,
				       year, TRUE, bm, n - 1843,
				       SPECIAL_VALUE, hd_elems, fday, count);
		      }
		    else
		      ocs_monthtext (init_data, detected, ptr_cc_id, d, m,
				     year, FALSE, bm, n - 1843, SPECIAL_VALUE,
				     hd_elems, fday, count);
		  }
		bdate += 19;
	      }
	  }
    }
}



void
celtic_hdy (init_data, detected, year, hd_elems, fday, count, cc)
     Bool *init_data;
     const Bool detected;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
     const char *cc;
/*!
   Manages all Celtic calendar based holidays.
     Calculations are done for a line at a definite meridian expressed as a
     time value.  This time value is given by the global `time_hour_offset'
     variable and by the global `time_min_offset' variable.  If HOUR and MIN
     are set to zero, calculations are made for Universal Time (UTC/GMT).  If
     HOUR and MIN have a positive sign, UTC/GMT calculations are made for
     meridians East of Greenwich, otherwise for meridians West of Greenwich.
*/
{
  register int cc_mode;
  auto int d;
  auto int m;
  auto int y = year;


  if (cc == (char *) NULL)
    cc_mode = 0;
  else if (!strcmp (cc, HD_MCNY))
    cc_mode = SPECIAL_VALUE;
  else
    abort ();			/* Error, invalid case! */
  ptr_cc_id = _("Cel");
  /*
     Manage all critical holidays, which can exceed the year bounds
     of a Julian/Gregorian calendar year.
   */
  if (sun_longitude
      (225.0, &d, &m, &y, time_hour_offset, time_min_offset,
       FALSE) != SPECIAL_VALUE)
    {
      holiday (*init_data, detected, _(hd_text[HD_CELTIC_SAMHAIN].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, d, m, y, hd_elems, fday, count);
      if (*init_data)
	*init_data = FALSE;
    }
  else
    y = year;
  if (!cc_mode)
    {
      if (sun_longitude
	  (315.0, &d, &m, &y, time_hour_offset, time_min_offset,
	   FALSE) != SPECIAL_VALUE)
	{
	  holiday (*init_data, detected, _(hd_text[HD_CELTIC_IMBOLG].ht_text),
		   ptr_cc_id, DIS_HLS_PREF, d, m, y, hd_elems, fday, count);
	  if (*init_data)
	    *init_data = FALSE;
	}
      else
	y = year;
      if (sun_longitude
	  (45.0, &d, &m, &y, time_hour_offset, time_min_offset,
	   FALSE) != SPECIAL_VALUE)
	{
	  holiday (*init_data, detected,
		   _(hd_text[HD_CELTIC_BELTANE].ht_text), ptr_cc_id,
		   DIS_HLS_PREF, d, m, y, hd_elems, fday, count);
	  if (*init_data)
	    *init_data = FALSE;
	}
      else
	y = year;
      if (sun_longitude
	  (135.0, &d, &m, &y, time_hour_offset, time_min_offset,
	   FALSE) != SPECIAL_VALUE)
	{
	  holiday (*init_data, detected,
		   _(hd_text[HD_CELTIC_LUGHNASA].ht_text), ptr_cc_id,
		   DIS_HLS_PREF, d, m, y, hd_elems, fday, count);
	  if (*init_data)
	    *init_data = FALSE;
	}
    }
}



void
chinese_hdy (init_data, detected, year, hd_elems, fday, count, cc,
	     is_any_meridian)
     Bool *init_data;
     const Bool detected;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
     const char *cc;
     const Bool is_any_meridian;
/*!
   Manages all Chinese calendar based holidays since AD 1645.
     Earlier years are not respected, because the rules of the Chinese
     calendar have changed too many times during these former years.
     The function results have to be interpreted with caution, because
     of the inaccuratenesses of the Moon phase and the Sun longitude
     calculation function used.
   For a good and detailed reference of the Chinese calendar, see:
     "The Mathematics of the Chinese Calendar" by Helmer Aslaksen,
     <http://www.math.nus.edu.sg/aslaksen/>.
*/
{
  if (year > 1644)
    {
      auto Ulint conjunction_vector[MONTH_MAX + 4];
      auto Ulint *ptr_conjunction_vector = conjunction_vector;
      auto Ulint zq;
      register int cj;
      register int lm;
      register int cc_mode;
      register int hour;
      register int min;
      register int extra = 0;
      register int i;
      register int j;
      register int k;
      auto int d;
      auto int m;
      auto int y = year;
      auto char *hdy_prefix;
      auto Bool is_checked = FALSE;
      auto Bool check_twice = FALSE;


      if (cc == (char *) NULL)
	cc_mode = 0;
      else if (!strcmp (cc, "BN"))
	cc_mode = 1;
      else if (!strcmp (cc, "CN"))
	cc_mode = 2;
      else if (!strcmp (cc, "HK"))
	cc_mode = 3;
      else if (!strcmp (cc, "ID"))
	cc_mode = 4;
      else if (!strcmp (cc, "KH"))
	cc_mode = 5;
      else if (!strcmp (cc, "KR"))
	cc_mode = 6;
      else if (!strcmp (cc, "KP"))
	cc_mode = 7;
      else if (!strcmp (cc, "LA"))
	cc_mode = 8;
      else if (!strcmp (cc, "MN"))
	cc_mode = 9;
      else if (!strcmp (cc, "MO"))
	cc_mode = 10;
      else if (!strcmp (cc, "MU"))
	cc_mode = 11;
      else if (!strcmp (cc, "MY"))
	cc_mode = 12;
      else if (!strcmp (cc, "PF"))
	cc_mode = 13;
      else if (!strcmp (cc, "SG"))
	cc_mode = 14;
      else if (!strcmp (cc, "TH"))
	cc_mode = 15;
      else if (!strcmp (cc, "TW"))
	cc_mode = 16;
      else if (!strcmp (cc, "VN"))
	cc_mode = 17;
      else if (!strcmp (cc, HD_MCNY))
	cc_mode = SPECIAL_VALUE;
      else
	abort ();		/* Error, invalid case! */
      if (!cc_mode || cc_mode == SPECIAL_VALUE)
	ptr_cc_id = _("Chi");
      if (is_any_meridian)
	{
	  /*
	     Astronomical calculations are done for a line at a definite
	     meridian expressed as a time value.  This time value is given
	     by the global `time_hour_offset' variable and by the global
	     `time_min_offset' variable.  If HOUR and MIN are set to zero,
	     calculations are made for Universal Time (UTC/GMT).  If HOUR
	     and MIN have a positive sign, UTC/GMT calculations are made
	     for meridians East of Greenwich, otherwise for meridians
	     West of Greenwich.
	   */
	  hour = time_hour_offset;
	  min = time_min_offset;
	}
      else
	{
	  /*
	     Astronomical calculations are based on China local time.
	   */
	  if (year < 1929)
	    {
	      /*
	         Meridian Beijing (116 25' degrees East), UTC/GMT time difference.
	       */
	      hour = 7;
	      min = 45;
	    }
	  else
	    {
	      /*
	         UTC/GMT-8 for the meridian 120 degrees East.
	       */
	      hour = 8;
	      min = 0;
	    }
	}
      if (cc_mode != SPECIAL_VALUE)
	{
	  /*
	     Manage the major solar term/Zhong-Qi related festivals first.
	   */
	  if (!cc_mode || cc_mode == 2 || cc_mode == 10)
	    {
	      if (cc_mode == 10)
		hdy_prefix = "+";
	      else
		hdy_prefix = DIS_HLS_PREF;
	      if (equinox_solstice (NH_WS, &d, &m, &y, hour, min) !=
		  SPECIAL_VALUE)
		{
		  holiday (*init_data, detected,
			   _(hd_text[HD_SOLSTICE_DAY].ht_text), ptr_cc_id,
			   hdy_prefix, d, m, y, hd_elems, fday, count);
		  if (*init_data)
		    *init_data = FALSE;
		}
	      else
		y = year;
	    }
	  /*
	     Manage the minor solar term/Jie-Qi related festivals next.
	   */
	  if (!cc_mode || cc_mode == 2 || cc_mode == 10)
	    j = 1;
	  else if (cc_mode == 3 || cc_mode == 16)
	    j = 2;
	  else
	    j = 0;
	  if (j)
	    {
	      if (cc_mode == 3 || cc_mode == 10 || cc_mode == 16)
		hdy_prefix = "+";
	      else
		hdy_prefix = DIS_HLS_PREF;
	      if (sun_longitude (15.0, &d, &m, &y, hour, min, FALSE) !=
		  SPECIAL_VALUE)
		{
		  if (j == 1)
		    holiday (*init_data, detected,
			     _(hd_text[HD_TOMB_SWEEPING_DAY].ht_text),
			     ptr_cc_id, hdy_prefix, d, m, y, hd_elems, fday,
			     count);
		  else
		    {
		      (void) next_date (&d, &m, &y);
		      holiday (*init_data, detected,
			       _(hd_text[HD_DAY_AFTER_TOMB_SWEEPING_DAY].
				 ht_text), ptr_cc_id, hdy_prefix, d, m, y,
			       hd_elems, fday, count);
		    }
		  if (*init_data)
		    *init_data = FALSE;
		}
	      else
		y = year;
	    }
	}
    LABEL_chinese_new_year_twice:
      /*
         And now the lunar based holidays, so check for a possible leap month.
       */
      lm =
	find_chinese_leap_month (ptr_conjunction_vector, &zq, y, hour, min);
      if (lm == SPECIAL_VALUE)
	{
	  /*
	     Error, cannot compute the lunisolar based calendar correctly.
	   */
	  if (warning_level >= 0)
	    {
	      sprintf (s2,
		       _
		       ("Cannot calculate lunisolar calendar for %d correctly"),
		       y);
	      print_text (stderr, s2);
	      if (warning_level >= WARN_LVL_MAX)
		my_error (ERR_INTERNAL_C_FUNC_FAILURE, __FILE__,
			  ((long) __LINE__) - 15L,
			  "find_chinese_leap_month()::lm!=", lm);
	    }
	}
      else
	{
	  /*
	     Now, since we know the leap month in the year YEAR, we can
	     calculate the date of the Chinese_New_Year/Chinese calendar
	     month 1 correctly.  The Chinese_New_Year is on the date of the
	     second New Moon ***after*** the date of the winter solstice
	     (which is always in the 11th month of the Chinese calendar)
	     of the previous YEAR, but only if there is NO leap month 11
	     or 12 (this case is marked by `lm==-11|-12'), otherwise it is
	     on the date of the third New Moon ***after*** the date of the
	     winter solstice of the previous YEAR.
	   */
	  cj = 0;
	  if (conjunction_vector[cj] == zq)
	    cj++;
	  if (lm < 0)
	    cj += 2;
	  else
	    cj++;
	  num2date (conjunction_vector[cj], &d, &m, &y);
	  /*
	     Detect whether it is necessary to respect the holidays of
	     the next YEAR.  This must always be done if we are creating
	     dates according to the proleptic Julian calendar and the
	     Chinese_New_Year holiday either started in the previous YEAR
	     or started early in the actual YEAR.
	   */
	  if (!is_checked
	      && ((y != year) || ((year <= greg->year) && (year >= 2700))))
	    is_checked = check_twice = TRUE;
	  if (!cc_mode || cc_mode == 6 || cc_mode == 16)
	    {
	      (void) prev_date (&d, &m, &y);
	      if (y == year)
		{
		  holiday (*init_data, detected,
			   _(hd_text[HD_CHINESE_NEW_YEARS_EVE].ht_text),
			   ptr_cc_id, (cc_mode) ? "+" : DIS_HLS_PREF, d, m, y,
			   hd_elems, fday, count);
		  if (*init_data)
		    *init_data = FALSE;
		}
	      (void) next_date (&d, &m, &y);
	    }
	  if (y == year)
	    {
	      if (!cc_mode
		  || cc_mode == 7
		  || cc_mode == 15 || cc_mode == SPECIAL_VALUE)
		hdy_prefix = DIS_HLS_PREF;
	      else if (cc_mode == 4 || cc_mode == 5)
		hdy_prefix = DIS_HLS_PREF2;
	      else
		hdy_prefix = "+";
	      /*
	         Calculate the year number used in the Chinese calendar,
	         based on the first historical record of the 60-year cycle
	         from 08-Mar-2637 BCE.
	       */
	      k = y + 2637 + extra;
	      sprintf (s2, "%s %d-%d",
		       _(hd_text[HD_CHINESE_NEW_YEARS_DAY].ht_text), k,
		       (lm < 0) ? 0 : lm);
	      holiday (*init_data, detected, s2, ptr_cc_id, hdy_prefix, d, m,
		       y, hd_elems, fday, count);
	      if (*init_data)
		*init_data = FALSE;
	      if (!cc_mode || cc_mode == SPECIAL_VALUE)
		{
		  /*
		     Compute the sexagesimal cycle data.
		   */
		  i = ((k - 1) / 60) + 1;
		  j = ((k - 1) % 60) + 1;
		  sprintf (s2, "%s %d/%02d-%02d %s-%s",
			   _(hd_text[HD_CHINESE_CYCLE].ht_text), i, j,
			   (lm < 0) ? 0 : lm, chi_stem[((j - 1) % 10)],
			   _(chi_branch[((j - 1) % 12)]));
		  holiday (*init_data, detected, s2, ptr_cc_id, DIS_HLS_PREF,
			   d, m, y, hd_elems, fday, count);
		}
	    }
	  if (cc_mode != SPECIAL_VALUE)
	    {
	      if (cc_mode == 1
		  || cc_mode == 4
		  || cc_mode == 5
		  || cc_mode == 8 || cc_mode == 11 || cc_mode == 13)
		j = 0;
	      else if (cc_mode == 6 || cc_mode == 12 || cc_mode == 14)
		j = 1;
	      else if (cc_mode == 2)
		j = 4;
	      else if (!cc_mode)
		j = 14;
	      else
		j = 2;
	      if (j)
		{
		  if (!cc_mode || cc_mode == 7 || cc_mode == 15)
		    hdy_prefix = DIS_HLS_PREF;
		  else
		    hdy_prefix = "+";
		  for (i = 0; i < j; i++)
		    {
		      (void) next_date (&d, &m, &y);
		      if (y == year)
			{
			  holiday (*init_data, detected,
				   _(hd_text[HD_CHINESE_NEW_YEARS_DAY].
				     ht_text), ptr_cc_id, hdy_prefix, d, m, y,
				   hd_elems, fday, count);
			  if (*init_data)
			    *init_data = FALSE;
			}
		    }
		}
	      if (!cc_mode)
		j = 1;
	      else if (cc_mode == 6 || cc_mode == 7)
		j = 3;
	      else
		j = 0;
	      if (j)
		{
		  if (!cc_mode || cc_mode == 6 || cc_mode == 7)
		    hdy_prefix = DIS_HLS_PREF;
		  else
		    hdy_prefix = "+";
		  num2date (conjunction_vector[cj] + 14, &d, &m, &y);
		  for (i = 0; i < j; i++)
		    {
		      if (y == year)
			{
			  holiday (*init_data, detected,
				   _(hd_text[HD_FESTIVAL_OF_LANTERNS].
				     ht_text), ptr_cc_id, hdy_prefix, d, m, y,
				   hd_elems, fday, count);
			  if (*init_data)
			    *init_data = FALSE;
			}
		      (void) next_date (&d, &m, &y);
		    }
		}
	      /*
	         Detect the date of the first day of the Chinese calendar
	         month 2.
	       */
	      if (lm == 1)
		cj++;
	      cj++;
	      if (!cc_mode)
		{
		  num2date (conjunction_vector[cj] + 14, &d, &m, &y);
		  if (y == year)
		    {
		      holiday (*init_data, detected,
			       _(hd_text[HD_LAO_TZES_BIRTHDAY].ht_text),
			       ptr_cc_id, DIS_HLS_PREF, d, m, y, hd_elems,
			       fday, count);
		      if (*init_data)
			*init_data = FALSE;
		    }
		}
	      /*
	         Detect the date of the first day of the Chinese calendar
	         month 4.
	       */
	      if (lm == 2 || lm == 3)
		cj++;
	      cj += 2;
	      num2date (conjunction_vector[cj] + 7, &d, &m, &y);
	      if (y == year)
		{
		  if (!cc_mode
		      || cc_mode == 3 || cc_mode == 6 || cc_mode == 7)
		    {
		      if (cc_mode == 3)
			hdy_prefix = "+";
		      else if (cc_mode == 6)
			{
			  if (year > 1974)
			    hdy_prefix = "+";
			  else
			    hdy_prefix = DIS_HLS_PREF;
			}
		      else
			hdy_prefix = DIS_HLS_PREF;
		      holiday (*init_data, detected,
			       _(hd_text[HD_BUDDHAS_BIRTHDAY].ht_text),
			       ptr_cc_id, hdy_prefix, d, m, y, hd_elems, fday,
			       count);
		      if (*init_data)
			*init_data = FALSE;
		    }
		}
	      /*
	         Detect the date of the first day of the Chinese calendar
	         month 5.
	       */
	      if (lm == 4)
		cj++;
	      num2date (conjunction_vector[++cj] + 4, &d, &m, &y);
	      if (y == year)
		{
		  if (cc_mode == 3 || cc_mode == 10)
		    hdy_prefix = "+";
		  else
		    hdy_prefix = DIS_HLS_PREF;
		  if (!cc_mode
		      || cc_mode == 2
		      || cc_mode == 3
		      || cc_mode == 6 || cc_mode == 7 || cc_mode == 10)
		    {
		      holiday (*init_data, detected,
			       _(hd_text[HD_DRAGON_BOAT_FESTIVAL].ht_text),
			       ptr_cc_id, hdy_prefix, d, m, y, hd_elems, fday,
			       count);
		      if (*init_data)
			*init_data = FALSE;
		    }
		}
	      /*
	         Detect the date of the first day of the Chinese calendar
	         month 7.
	       */
	      if (lm == 5 || lm == 6)
		cj++;
	      cj += 2;
	      if (!cc_mode)
		{
		  num2date (conjunction_vector[cj] + 6, &d, &m, &y);
		  if (y == year)
		    {
		      holiday (*init_data, detected,
			       _(hd_text[HD_STAR_FESTIVAL].ht_text),
			       ptr_cc_id, DIS_HLS_PREF, d, m, y, hd_elems,
			       fday, count);
		      if (*init_data)
			*init_data = FALSE;
		    }
		}
	      /*
	         Detect the date of the first day of the Chinese calendar
	         month 8.
	       */
	      if (lm == 7)
		cj++;
	      num2date (conjunction_vector[++cj] + 13, &d, &m, &y);
	      if (cc_mode == 6 || cc_mode == 7 || cc_mode == 10)
		hdy_prefix = "+";
	      else
		hdy_prefix = DIS_HLS_PREF;
	      if ((cc_mode == 6) && (y == year))
		{
		  holiday (*init_data, detected,
			   _(hd_text[HD_MID_AUTUMN_FESTIVAL].ht_text),
			   ptr_cc_id, hdy_prefix, d, m, y, hd_elems, fday,
			   count);
		  if (*init_data)
		    *init_data = FALSE;
		}
	      if (!cc_mode || cc_mode == 2 || cc_mode == 10)
		j = 1;
	      else if (cc_mode == 6)
		j = 2;
	      else if (cc_mode == 7)
		j = 3;
	      else
		j = 0;
	      for (i = 0; i < j; i++)
		{
		  (void) next_date (&d, &m, &y);
		  if (y == year)
		    {
		      holiday (*init_data, detected,
			       _(hd_text[HD_MID_AUTUMN_FESTIVAL].ht_text),
			       ptr_cc_id, hdy_prefix, d, m, y, hd_elems, fday,
			       count);
		      if (*init_data)
			*init_data = FALSE;
		    }
		}
	      if (cc_mode == 3 || cc_mode == 10)
		{
		  num2date (conjunction_vector[cj] + 15, &d, &m, &y);
		  if (y == year)
		    {
		      holiday (*init_data, detected,
			       _(hd_text[HD_DAY_AFTER_MID_AUTUMN_FESTIVAL].
				 ht_text), ptr_cc_id, "+", d, m, y, hd_elems,
			       fday, count);
		      if (*init_data)
			*init_data = FALSE;
		    }
		}
	      if (!cc_mode)
		{
		  num2date (conjunction_vector[cj] + 26, &d, &m, &y);
		  if (y == year)
		    {
		      holiday (*init_data, detected,
			       _(hd_text[HD_CONFUCIUS_BIRTHDAY].ht_text),
			       ptr_cc_id, DIS_HLS_PREF, d, m, y, hd_elems,
			       fday, count);
		      if (*init_data)
			*init_data = FALSE;
		    }
		}
	      /*
	         Detect the date of the first day of the Chinese calendar
	         month 9.
	       */
	      if (lm == 8)
		cj++;
	      cj++;
	      if (!cc_mode || cc_mode == 3)
		{
		  num2date (conjunction_vector[cj] + 8, &d, &m, &y);
		  if (y == year)
		    {
		      holiday (*init_data, detected,
			       _(hd_text[HD_DOUBLE_9_DAY].ht_text), ptr_cc_id,
			       (cc_mode) ? "+" : DIS_HLS_PREF, d, m, y,
			       hd_elems, fday, count);
		      if (*init_data)
			*init_data = FALSE;
		    }
		}
	      /*
	         Detect the date of the first day of the Chinese calendar
	         month 10.
	       */
	      if (lm == 9)
		cj++;
	      cj++;
	      if (!cc_mode)
		{
		  num2date (conjunction_vector[cj] + 5, &d, &m, &y);
		  if (y == year)
		    {
		      holiday (*init_data, detected,
			       _(hd_text[HD_BODHIDHARMAS_BIRTHDAY].ht_text),
			       ptr_cc_id, DIS_HLS_PREF, d, m, y, hd_elems,
			       fday, count);
		      if (*init_data)
			*init_data = FALSE;
		    }
		}
	    }
	  if (check_twice)
	    {
	      check_twice = FALSE;
	      y = year + 1;
	      extra++;
	      goto LABEL_chinese_new_year_twice;
	    }
	}
    }
}



void
chinese_mth (init_data, detected, year, hd_elems, fday, count,
	     is_any_meridian, do_chinese)
     Bool *init_data;
     const Bool detected;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
     const Bool is_any_meridian;
     const Bool do_chinese;
/*!
   Manages all Chinese/Japanese calendar based months since AD 1645.
     Earlier years are not respected, because the rules of the Chinese/Japanese
     calendar have changed too many times during these former years.  The
     function results have to be interpreted with caution, because of the
     inaccuratenesses of the Moon phase and the Sun longitude calculation
     function used.
   For a good and detailed reference of the Chinese calendar, see:
     "The Mathematics of the Chinese Calendar" by Helmer Aslaksen,
     <http://www.math.nus.edu.sg/aslaksen/>.
*/
{
  if (year > 1644)
    {
      auto Ulint conjunction_vector[MONTH_MAX + 4];
      auto Ulint *ptr_conjunction_vector = conjunction_vector;
      auto Ulint zq;
      auto Ulint cj_buf = 0L;
      register int cj;
      register int lm_buf = 0;
      register int lm;
      register int cny;
      register int cm;
      register int hour;
      register int min;
      register int i;
      register int j = 0;
      register int n;
      register int n_max;
      auto int d;
      auto int m;
      auto int y;


      if (do_chinese)
	ptr_cc_id = _("Chi");
      else
	ptr_cc_id = _("Jap");
      if (is_any_meridian)
	{
	  /*
	     Astronomical calculations are done for a line at a definite
	     meridian expressed as a time value.  This time value is given
	     by the global `time_hour_offset' variable and by the global
	     `time_min_offset' variable.  If HOUR and MIN are set to zero,
	     calculations are made for Universal Time (UTC/GMT).  If HOUR
	     and MIN have a positive sign, UTC/GMT calculations are made
	     for meridians East of Greenwich, otherwise for meridians
	     West of Greenwich.
	   */
	  hour = time_hour_offset;
	  min = time_min_offset;
	}
      else
	{
	  if (do_chinese)
	    {
	      /*
	         Astronomical calculations are based on China local time.
	       */
	      if (year < 1929)
		{
		  /*
		     Meridian Beijing (116 25' degrees East), UTC/GMT time difference.
		   */
		  hour = 7;
		  min = 45;
		}
	      else
		{
		  /*
		     UTC/GMT-8 for the meridian 120 degrees East.
		   */
		  hour = 8;
		  min = 0;
		}
	    }
	  else
	    {
	      /*
	         Astronomical calculations are based on China/Japan local time.
	       */
	      if (year < 1888)
		{
		  /*
		     Meridian Beijing (116 25' degrees East), UTC/GMT time difference.
		   */
		  hour = 7;
		  min = 45;
		}
	      else
		{
		  /*
		     UTC/GMT-9 for the meridian 135 degrees East.
		   */
		  hour = 9;
		  min = 0;
		}
	    }
	}
      /*
         Detect whether it is necessary to respect the holidays of the
         next YEAR instead of the previous YEAR.  This must always be
         done if we are creating dates according to the proleptic Julian
         calendar and the Chinese_New_Year holiday either started in
         the previous YEAR or started early in the actual YEAR.
       */
      if ((year <= greg->year) && (year >= 2700))
	{
	  n = year;
	  n_max = year + 1;
	}
      else
	{
	  n = year - 1;
	  n_max = year;
	}
      for (; n <= n_max; n++)
	{
	  y = n;
	  /*
	     Manage the lunar based months, so check for a possible leap month.
	   */
	  lm =
	    find_chinese_leap_month (ptr_conjunction_vector, &zq, y, hour,
				     min);
	  if (lm == SPECIAL_VALUE)
	    {
	      /*
	         Error, cannot compute the lunisolar based calendar correctly.
	       */
	      if (warning_level >= 0)
		{
		  sprintf (s2,
			   _
			   ("Cannot calculate lunisolar calendar for %d correctly"),
			   y);
		  print_text (stderr, s2);
		  if (warning_level >= WARN_LVL_MAX)
		    my_error (ERR_INTERNAL_C_FUNC_FAILURE, __FILE__,
			      ((long) __LINE__) - 15L,
			      "find_chinese_leap_month()::lm!=", lm);
		}
	    }
	  else
	    {
	      cj = 0;
	      if (conjunction_vector[cj] == zq)
		cj++;
	      if (lm < 0)
		cj += 2;
	      else
		cj++;
	      if (!lm)
		lm = YEAR_MAX;
	      cny = cj;
	      /*
	         And manage all Chinese/Japanese calendar months.
	       */
	      for (cm = MONTH_MIN, i = 0; i <= MONTH_MAX; cm++, i++)
		{
		  num2date (conjunction_vector[cj++], &d, &m, &y);
		  if (cm > MONTH_MAX)
		    {
		      if (!cj_buf)
			{
			  lm_buf = lm;
			  cj_buf = conjunction_vector[cj - 1];
			}
		    }
		  else if (y == year)
		    {
		      if (do_chinese)
			/*
			   Calculate the year number used in the Chinese calendar,
			   based on the first historical record of the 60-year
			   cycle from 08-Mar-2637 BCE.
			 */
			j = n + 2637;
		      else
			/*
			   Calculate the year number used in the Japanese calendar,
			   based on the first historical record of the 60-year
			   cycle from 660 BCE.
			 */
			j = n + 660;
		      if (i == lm)
			{
			  cm--;
			  ocs_monthtext (init_data, detected, ptr_cc_id, d, m,
					 year, TRUE, cm, j, SPECIAL_VALUE,
					 hd_elems, fday, count);
			}
		      else
			ocs_monthtext (init_data, detected, ptr_cc_id, d, m,
				       year, FALSE, cm, j, SPECIAL_VALUE,
				       hd_elems, fday, count);
		    }
		}
	      if (cj_buf)
		if (cj_buf < conjunction_vector[cny])
		  {
		    num2date (cj_buf, &d, &m, &y);
		    if (y == year)
		      ocs_monthtext (init_data, detected, ptr_cc_id, d, m,
				     year, (lm_buf == MONTH_MAX), MONTH_MAX,
				     j - 1, SPECIAL_VALUE, hd_elems, fday,
				     count);
		  }
	    }
	}
    }
}



void
christian_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all Christian Western churches calendar based holidays.
*/
{
  register int day;
  register int month;


  ptr_cc_id = _("Chr");
  holiday (*init_data, detected, _(hd_text[HD_1ST_SUNDAY_IN_LENT].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter - 42, 0, year, hd_elems, fday,
	   count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_2ND_SUNDAY_IN_LENT].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter - 35, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_3RD_SUNDAY_IN_LENT].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter - 28, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_4TH_SUNDAY_IN_LENT].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter - 21, 0, year, hd_elems, fday,
	   count);
  day = eval_holiday (24, MONTH_MAX, year, DAY_MAX, FALSE);
  holiday (*init_data, detected, _(hd_text[HD_4TH_ADVENT].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, MONTH_MAX, year, hd_elems, fday,
	   count);
  day -= DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_3RD_ADVENT].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, MONTH_MAX, year, hd_elems, fday,
	   count);
  day -= DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_2ND_ADVENT].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, MONTH_MAX, year, hd_elems, fday,
	   count);
  day -= DAY_MAX;
  if (day > 0)
    month = MONTH_MAX;
  else
    {
      day = dvec[11 - 1] + day;
      month = 11;
    }
  holiday (*init_data, detected, _(hd_text[HD_1ST_ADVENT].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, month, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SOULS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 2, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ASH_WEDNESDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter - 46, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_ST_BARTHOLOMEW_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, 26, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 25, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter + 39, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter + 1, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_CORPUS_CHRISTI].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, easter + 60, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_FEAST_OF_HEART_JESUS].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter + 68, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter - 2, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_SATURDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter - 1, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_MARTINIMAS].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, 11, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARYS_ANNUNCIATION_DAY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, 25, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARY_BLESSED_VIRGIN].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, DAY_MIN, MONTH_MIN, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_CANDLEMAS].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 2, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_EXPECTATION].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 18, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARYS_IMMACULATE_CONCEPTION].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, 8, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_NATIVITY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 8, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_NAME].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, 12, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_MATERNITY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 11, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_SACRIFICE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 21, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_VISITATION].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 2, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MIDSUMMER_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_PALM_SUNDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter - DAY_MAX, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_PASSION_SUNDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter - 14, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_PENTECOST].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, easter + 49, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_QUINQUAGESIMA_SUNDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter - 49, 0, year, hd_elems, fday,
	   count);
  if (year > 1516)
    holiday (*init_data, detected, _(hd_text[HD_REFORMATION_DAY].ht_text),
	     ptr_cc_id, DIS_HLS_PREF, dvec[10 - 1], 10, year, hd_elems, fday,
	     count);
  holiday (*init_data, detected, _(hd_text[HD_ROGATION_SUNDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter + 35, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_SEPTUAGESIMA_SUNDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter - 63, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_SEXAGESIMA_SUNDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter - 56, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_ST_ANDREWS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, dvec[11 - 1], 11, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_ST_JOSEPHS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 19, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_LAURENTIUS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 10, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_NICHOLAS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 6, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_ST_PETER_AND_ST_PAUL].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 29, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_VALENTINES_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 14, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 6, MONTH_MIN, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_HOLY_TRINITY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter + 56, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_WHIT_MONDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter + 50, 0, year, hd_elems, fday,
	   count);
}



void
coptic_mth (init_data, detected, year, hd_elems, fday, count, do_coptic)
     Bool *init_data;
     const Bool detected;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
     const Bool do_coptic;
/*!
   Manages all Coptic/Ethiopic calendar based months
     since AD 284 (Coptic) respectively AD 7 (Ethiopic).
*/
{
  if (!do_coptic || (do_coptic && (year > 283)))
    {
      auto Ulint cdate;
      register int i;
      register int n;
      auto int d;
      auto int m;
      auto int y;


      if (do_coptic)
	ptr_cc_id = "EG";
      else
	ptr_cc_id = "ET";
      for (n = year - 1; n <= year; n++)
	if (!do_coptic || (do_coptic && (n > 283)))
	  {
	    cdate = date2num (29, 8, n)
	      + ((n & 3) == 3) + julian_gregorian_diff (29, 8, n);
	    /*
	       And manage all Coptic/Ethiopic calendar months.
	     */
	    for (i = 0; i <= MONTH_MAX; i++)
	      {
		num2date (cdate, &d, &m, &y);
		if (y == year)
		  ocs_monthtext (init_data, detected, ptr_cc_id, d, m, year,
				 (i == MONTH_MAX),
				 (i == MONTH_MAX) ? i : i + 1,
				 (do_coptic) ? n - 283 : n - 7, SPECIAL_VALUE,
				 hd_elems, fday, count);
		cdate += 30;
	      }
	  }
    }
}



void
french_revolutionary_mth (init_data, detected, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all French Revolutionary (Republican) calendar months
     since AD 22-Sep-1792.
*/
{
  if (year > 1791)
    {
      auto Ulint fdate;
      register int i;
      register int n;
      auto int d;
      auto int m;
      auto int y;


      ptr_cc_id = _("FRR");
      for (n = year - 1; n <= year; n++)
	if (n > 1791)
	  {
	    y = n;
	    /*
	       Detect the French_Revolutionary_New_Year's_Day as used in the
	       French Revolutionary (Republican) calendar, which is based
	       on the date of the autumnal equinox as it occurs in Paris, FR.
	     */
	    fdate = (Ulint) equinox_solstice (NH_AE, &d, &m, &y, 0, 9);
	    if (fdate != (Ulint) SPECIAL_VALUE)
	      {
		/*
		   And manage all French Revolutionary calendar months.
		 */
		for (i = 0; i <= MONTH_MAX; i++)
		  {
		    num2date (fdate, &d, &m, &y);
		    if (y == year)
		      ocs_monthtext (init_data, detected, ptr_cc_id, d, m,
				     year, (i == MONTH_MAX),
				     (i == MONTH_MAX) ? i : i + 1, n - 1791,
				     SPECIAL_VALUE, hd_elems, fday, count);
		    fdate += 30;
		  }
	      }
	  }
    }
}



void
indian_civil_mth (init_data, detected, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all Indian civil-calendar (reformed calendar) months
     since AD 22-Mar-1957.
*/
{
  if (year > 1956)
    {
      auto Ulint idate;
      register int i;
      register int n;
      auto int d;
      auto int m;
      auto int y;


      ptr_cc_id = "IN";
      for (n = year - 1; n <= year; n++)
	if (n > 1956)
	  {
	    d = 22 - (days_of_february (n) == 29);
	    if (n <= greg->year)
	      d -= 13;
	    idate = date2num (d, 3, n);
	    /*
	       And manage all Indian civil-calendar months.
	     */
	    for (i = MONTH_MIN; i <= MONTH_MAX; i++)
	      {
		num2date (idate, &d, &m, &y);
		if (y == year)
		  ocs_monthtext (init_data, detected, ptr_cc_id, d, m, year,
				 FALSE, i, n - 78, SPECIAL_VALUE, hd_elems,
				 fday, count);
		if (i == MONTH_MIN)
		  idate += ((days_of_february (n) == 29) ? 31 : 30);
		else
		  idate += ((i < 7) ? 31 : 30);
	      }
	  }
    }
}



void
hebrew_hdy (init_data, detected, year, hd_elems, fday, count, cc)
     Bool *init_data;
     const Bool detected;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
     const char *cc;
/*!
   Manages all Hebrew calendar based holidays.
     (Really unoptimized and dull implementation of the arithmetical
     lunisolar Hebrew calendar, but it works.)
*/
{
  auto Ulint ddiff;
  auto Ulint hdate;
  register int cc_mode;
  register int doy;
  register int hm;
  register int i;
  register int n;
  auto int d;
  auto int m;
  auto int y;


  if (cc == (char *) NULL)
    cc_mode = 0;
  else if (!strcmp (cc, "IL"))
    cc_mode = 1;
  else if (!strcmp (cc, HD_MCNY))
    cc_mode = SPECIAL_VALUE;
  else
    abort ();			/* Error, invalid case! */
  if (!cc_mode || cc_mode == SPECIAL_VALUE)
    ptr_cc_id = _("Heb");
  /*
     Manage those holidays first, which either cannot exceed the year bounds
     of a Julian/Gregorian calendar year, or do not depend on the length of
     the Hebrew calendar year.
   */
  doy = tishri_1 (year) - 1;
  /*
     Calculate the year number used in the Hebrew calendar.
   */
  d = doy + 1;
  sprintf (s2, "%s %d", _(hd_text[HD_HEB_ROSH_HASHANA].ht_text), year + 3761);
  holiday (*init_data, detected, s2, ptr_cc_id,
	   (cc_mode > 0) ? "+" : DIS_HLS_PREF, d, 0, year, hd_elems, fday,
	   count);
  if (*init_data)
    *init_data = FALSE;
  if (cc_mode != SPECIAL_VALUE)
    {
      holiday (*init_data, detected, _(hd_text[HD_HEB_ROSH_HASHANA].ht_text),
	       ptr_cc_id, (cc_mode) ? "+" : DIS_HLS_PREF, d + 1, 0, year,
	       hd_elems, fday, count);
      (void) doy2date (doy - 143, (days_of_february (year) == 29), &d, &m);
      i = weekday_of_date (d, m, year);
      if (i == 4)
	(void) doy2date (doy - 144, (days_of_february (year) == 29), &d, &m);
      else if (i == 5)
	(void) doy2date (doy - 145, (days_of_february (year) == 29), &d, &m);
      holiday (*init_data, detected, _(hd_text[HD_HEB_YOM_HAZIKARON].ht_text),
	       ptr_cc_id, (cc_mode) ? "#" : DIS_HLS_PREF, d, m, year,
	       hd_elems, fday, count);
      d = doy - 162;
      if (!cc_mode)
	for (i = 0; i < 7; i++)
	  holiday (*init_data, detected, _(hd_text[HD_HEB_PESACH].ht_text),
		   ptr_cc_id, DIS_HLS_PREF, d + i, 0, year, hd_elems, fday,
		   count);
      else
	{
	  holiday (*init_data, detected, _(hd_text[HD_HEB_PESACH].ht_text),
		   ptr_cc_id, "+", d, 0, year, hd_elems, fday, count);
	  holiday (*init_data, detected, _(hd_text[HD_HEB_PESACH].ht_text),
		   ptr_cc_id, "+", d + 6, 0, year, hd_elems, fday, count);
	}
      if (year > 1942)
	{
	  (void) doy2date (doy - 150, (days_of_february (year) == 29), &d,
			   &m);
	  if (weekday_of_date (d, m, year) == 5)
	    (void) doy2date (doy - 151, (days_of_february (year) == 29), &d,
			     &m);
	  holiday (*init_data, detected,
		   _(hd_text[HD_HEB_YOM_HASHOAH].ht_text), ptr_cc_id,
		   (cc_mode) ? "#" : DIS_HLS_PREF, d, m, year, hd_elems, fday,
		   count);
	}
      (void) doy2date (doy - 142, (days_of_february (year) == 29), &d, &m);
      i = weekday_of_date (d, m, year);
      if (i == 5)
	(void) doy2date (doy - 143, (days_of_february (year) == 29), &d, &m);
      else if (i == 6)
	(void) doy2date (doy - 144, (days_of_february (year) == 29), &d, &m);
      if (year > 1947)
	holiday (*init_data, detected,
		 _(hd_text[HD_HEB_YOM_HA_AZMAUT].ht_text), ptr_cc_id,
		 (cc_mode) ? "+" : DIS_HLS_PREF, d, m, year, hd_elems, fday,
		 count);
      holiday (*init_data, detected, _(hd_text[HD_HEB_LAG_BOMER].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, doy - 129, 0, year, hd_elems, fday,
	       count);
      if (year > 1966)
	holiday (*init_data, detected,
		 _(hd_text[HD_HEB_YOM_YERUSHALAYIM].ht_text), ptr_cc_id,
		 DIS_HLS_PREF, doy - 119, 0, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_HEB_SHAVUOT].ht_text),
	       ptr_cc_id, (cc_mode) ? "+" : DIS_HLS_PREF, doy - 112, 0, year,
	       hd_elems, fday, count);
      (void) doy2date (doy - 50, (days_of_february (year) == 29), &d, &m);
      if (weekday_of_date (d, m, year) == 6)
	(void) doy2date (doy - 49, (days_of_february (year) == 29), &d, &m);
      holiday (*init_data, detected, _(hd_text[HD_HEB_TISHA_BAV].ht_text),
	       ptr_cc_id, (cc_mode) ? "#" : DIS_HLS_PREF, d, m, year,
	       hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_HEB_YOM_KIPPUR].ht_text),
	       ptr_cc_id, (cc_mode) ? "+" : DIS_HLS_PREF, doy + 10, 0, year,
	       hd_elems, fday, count);
      d = doy + 15;
      if (!cc_mode)
	for (i = 0; i < 7; i++)
	  holiday (*init_data, detected, _(hd_text[HD_HEB_SUKKOT].ht_text),
		   ptr_cc_id, DIS_HLS_PREF, d + i, 0, year, hd_elems, fday,
		   count);
      else
	{
	  holiday (*init_data, detected, _(hd_text[HD_HEB_SUKKOT].ht_text),
		   ptr_cc_id, "+", d, 0, year, hd_elems, fday, count);
	  holiday (*init_data, detected, _(hd_text[HD_HEB_SUKKOT].ht_text),
		   ptr_cc_id, "+", d + 6, 0, year, hd_elems, fday, count);
	}
      holiday (*init_data, detected, _(hd_text[HD_HEB_SIMCHAT_TORAH].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, doy + 22, 0, year, hd_elems, fday,
	       count);
      /*
         Manage those holidays last, which can either exceed the year
         bounds of a Julian/Gregorian calendar year, or depend on
         the length of the Hebrew calendar year.
       */
      for (n = year - 1; n <= year; n++)
	{
	  /*
	     Detect the length of the Hebrew calendar year.
	   */
	  (void) doy2date (tishri_1 (n + 1), (days_of_february (n + 1) == 29),
			   &d, &m);
	  ddiff = date2num (d, m, n + 1);
	  doy = tishri_1 (n) - 1;
	  (void) doy2date (doy + 1, (days_of_february (n) == 29), &d, &m);
	  hdate = date2num (d, m, n);
	  ddiff -= hdate--;
	  /*
	     Calculate the selector for the proper number of past days of month.
	   */
	  if (ddiff < 383)
	    hm = (int) ddiff - 353;
	  else
	    hm = (int) ddiff - 380;
	  /*
	     And manage all ``critical'' holidays.
	   */
	  for (i = 0; i < 8; i++)
	    {
	      num2date (hdate + hmvec[hm][3 - 1] + 25 + i, &d, &m, &y);
	      if (y == year)
		holiday (*init_data, detected,
			 _(hd_text[HD_HEB_HANNUKAH].ht_text), ptr_cc_id,
			 DIS_HLS_PREF, d, m, year, hd_elems, fday, count);
	    }
	  num2date (hdate + hmvec[hm][5 - 1] + 15, &d, &m, &y);
	  if (y == year)
	    holiday (*init_data, detected,
		     _(hd_text[HD_HEB_TU_BSHEVAT].ht_text), ptr_cc_id,
		     DIS_HLS_PREF, d, m, year, hd_elems, fday, count);
	  num2date (hdate + hmvec[hm][7 - 1] + 14, &d, &m, &y);
	  if (y == year)
	    holiday (*init_data, detected, _(hd_text[HD_HEB_PURIM].ht_text),
		     ptr_cc_id, DIS_HLS_PREF, d, m, year, hd_elems, fday,
		     count);
	  num2date (hdate + hmvec[hm][7 - 1] + 15, &d, &m, &y);
	  if (y == year)
	    holiday (*init_data, detected,
		     _(hd_text[HD_HEB_SHUSHAN_PURIM].ht_text), ptr_cc_id,
		     DIS_HLS_PREF, d, m, year, hd_elems, fday, count);
	}
    }
}



void
hebrew_mth (init_data, detected, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all Hebrew calendar based months.
     (Really unoptimized and dull implementation of the arithmetical
     lunisolar Hebrew calendar, but it works.)
*/
{
  auto Ulint ddiff;
  auto Ulint hdate;
  register int hms;
  register int hm;
  register int i;
  register int n;
  auto int d;
  auto int m;
  auto int y;

  ptr_cc_id = _("Heb");
  for (n = year - 1; n <= year; n++)
    {
      /*
         Detect the length of the Hebrew calendar year.
       */
      (void) doy2date (tishri_1 (n + 1), (days_of_february (n + 1) == 29), &d,
		       &m);
      ddiff = date2num (d, m, n + 1);
      (void) doy2date (tishri_1 (n), (days_of_february (n) == 29), &d, &m);
      hdate = date2num (d, m, n);
      ddiff -= hdate;
      /*
         Calculate the selector for the proper number of past days of month.
       */
      if (ddiff < 383)
	hms = (int) ddiff - 353;
      else
	hms = (int) ddiff - 380;
      /*
         And manage all Hebrew calendar months.
       */
      for (hm = MONTH_MIN, i = 0; i <= MONTH_MAX; hm++, i++)
	{
	  num2date (hdate + hmvec[hms][i], &d, &m, &y);
	  if (y == year)
	    {
	      if (i == 6)
		{
		  hm--;
		  if (hmvec[hms][i] != hmvec[hms][i - 1])
		    ocs_monthtext (init_data, detected, ptr_cc_id, d, m, year,
				   TRUE, ((hm + 5) % MONTH_MAX) + 1, n + 3761,
				   SPECIAL_VALUE, hd_elems, fday, count);
		}
	      else
		ocs_monthtext (init_data, detected, ptr_cc_id, d, m, year,
			       FALSE, ((hm + 5) % MONTH_MAX) + 1, n + 3761,
			       SPECIAL_VALUE, hd_elems, fday, count);
	    }
	}
    }
}



void
islamic_hdy (init_data, detected, year, hd_elems, fday, count, cc)
     Bool *init_data;
     const Bool detected;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
     const char *cc;
/*!
   Manages all Islamic civil-calendar based holidays since AD 622.
     Note that this function uses the arithmetical lunar Islamic ***CIVIL***
     calendar.  This kind of calendar is just a rough estimate of the
     religious Islamic lunar calendar, which is calculated astronomically.
     So don't be surprised if some holiday dates differ from the religious
     Islamic calendar about +/-1...2 days in some years.
*/
{
  if (year > 621)
    {
      auto Ulint idate;
      register int cc_mode = YEAR_MAX;
      register int i;
      register int j;
      register int n;
      register int doy;
      auto int doy2;
      auto int d;
      auto int m;
      auto int y;
      auto char *hdy_prefix;
      auto Bool check_twice = FALSE;


      if (cc == (char *) NULL)
	cc_mode = 0;
      else
	{
	  if (!strcmp (cc, "AE"))
	    cc_mode = 1;
	  if (!strcmp (cc, "AF"))
	    cc_mode = 2;
	  if (!strcmp (cc, "AL"))
	    cc_mode = 3;
	  if (!strcmp (cc, "AZ"))
	    cc_mode = 4;
	  if (!strcmp (cc, "BA"))
	    cc_mode = 5;
	  if (!strcmp (cc, "BD"))
	    cc_mode = 6;
	  if (!strcmp (cc, "BF"))
	    cc_mode = 7;
	  if (!strcmp (cc, "BH"))
	    cc_mode = 8;
	  if (!strcmp (cc, "BJ"))
	    cc_mode = 9;
	  if (!strcmp (cc, "BN"))
	    cc_mode = 10;
	  if (!strcmp (cc, "CI"))
	    cc_mode = 11;
	  if (!strcmp (cc, "CM"))
	    cc_mode = 12;
	  if (!strcmp (cc, "DJ"))
	    cc_mode = 13;
	  if (!strcmp (cc, "DZ"))
	    cc_mode = 14;
	  if (!strcmp (cc, "EG"))
	    cc_mode = 15;
	  if (!strcmp (cc, "ER"))
	    cc_mode = 16;
	  if (!strcmp (cc, "ET"))
	    cc_mode = 17;
	  if (!strcmp (cc, "FJ"))
	    cc_mode = 18;
	  if (!strcmp (cc, "GE"))
	    cc_mode = 19;
	  if (!strcmp (cc, "GH"))
	    cc_mode = 20;
	  if (!strcmp (cc, "GM"))
	    cc_mode = 21;
	  if (!strcmp (cc, "GN"))
	    cc_mode = 22;
	  if (!strcmp (cc, "GW"))
	    cc_mode = 23;
	  if (!strcmp (cc, "GY"))
	    cc_mode = 24;
	  if (!strcmp (cc, "ID"))
	    cc_mode = 25;
	  if (!strcmp (cc, "IN"))
	    cc_mode = 26;
	  if (!strcmp (cc, "IQ"))
	    cc_mode = 27;
	  if (!strcmp (cc, "IR"))
	    cc_mode = 28;
	  if (!strcmp (cc, "JO"))
	    cc_mode = 29;
	  if (!strcmp (cc, "KE"))
	    cc_mode = 30;
	  if (!strcmp (cc, "KG"))
	    cc_mode = 31;
	  if (!strcmp (cc, "KM"))
	    cc_mode = 32;
	  if (!strcmp (cc, "KW"))
	    cc_mode = 33;
	  if (!strcmp (cc, "KZ"))
	    cc_mode = 34;
	  if (!strcmp (cc, "LB"))
	    cc_mode = 35;
	  if (!strcmp (cc, "LK"))
	    cc_mode = 36;
	  if (!strcmp (cc, "LY"))
	    cc_mode = 37;
	  if (!strcmp (cc, "MA") || !strcmp (cc, "EH"))
	    cc_mode = 38;
	  if (!strcmp (cc, "MK"))
	    cc_mode = 39;
	  if (!strcmp (cc, "ML"))
	    cc_mode = 40;
	  if (!strcmp (cc, "MR"))
	    cc_mode = 41;
	  if (!strcmp (cc, "MU"))
	    cc_mode = 42;
	  if (!strcmp (cc, "MV"))
	    cc_mode = 43;
	  if (!strcmp (cc, "MY"))
	    cc_mode = 44;
	  if (!strcmp (cc, "NE"))
	    cc_mode = 45;
	  if (!strcmp (cc, "NG"))
	    cc_mode = 46;
	  if (!strcmp (cc, "NM"))
	    cc_mode = 47;
	  if (!strcmp (cc, "OM"))
	    cc_mode = 48;
	  if (!strcmp (cc, "PK"))
	    cc_mode = 49;
	  if (!strcmp (cc, "QA"))
	    cc_mode = 50;
	  if (!strcmp (cc, "RW"))
	    cc_mode = 51;
	  if (!strcmp (cc, "SA"))
	    cc_mode = 52;
	  if (!strcmp (cc, "SD"))
	    cc_mode = 53;
	  if (!strcmp (cc, "SG"))
	    cc_mode = 54;
	  if (!strcmp (cc, "SL"))
	    cc_mode = 55;
	  if (!strcmp (cc, "SO"))
	    cc_mode = 56;
	  if (!strcmp (cc, "SR"))
	    cc_mode = 57;
	  if (!strcmp (cc, "SY"))
	    cc_mode = 58;
	  if (!strcmp (cc, "TD"))
	    cc_mode = 59;
	  if (!strcmp (cc, "TG"))
	    cc_mode = 60;
	  if (!strcmp (cc, "TJ"))
	    cc_mode = 61;
	  if (!strcmp (cc, "TM"))
	    cc_mode = 62;
	  if (!strcmp (cc, "TN"))
	    cc_mode = 63;
	  if (!strcmp (cc, "TR"))
	    cc_mode = 64;
	  if (!strcmp (cc, "TT"))
	    cc_mode = 65;
	  if (!strcmp (cc, "TZ"))
	    cc_mode = 66;
	  if (!strcmp (cc, "UG"))
	    cc_mode = 67;
	  if (!strcmp (cc, "UZ"))
	    cc_mode = 68;
	  if (!strcmp (cc, "YE"))
	    cc_mode = 69;
	  if (!strcmp (cc, "YU"))
	    cc_mode = 70;
	  if (!strcmp (cc, HD_MCNY))
	    cc_mode = SPECIAL_VALUE;
	}
      if (cc_mode == YEAR_MAX)
	abort ();		/* Error, invalid case! */
      if (!cc_mode || cc_mode == SPECIAL_VALUE)
	ptr_cc_id = _("Isl");
      /*
         Manage those holidays first, which either cannot exceed
         the year bounds of a Julian/Gregorian calendar year.
       */
      doy = muharram_1 (year, &doy2);
      if (!cc_mode
	  || cc_mode == 3
	  || cc_mode == 4
	  || cc_mode == 7
	  || cc_mode == 9
	  || cc_mode == 11
	  || cc_mode == 12
	  || cc_mode == 13
	  || cc_mode == 16
	  || cc_mode == 17
	  || cc_mode == 18
	  || cc_mode == 19
	  || cc_mode == 20
	  || cc_mode == 21
	  || cc_mode == 24
	  || cc_mode == 26
	  || cc_mode == 30
	  || cc_mode == 36
	  || cc_mode == 39
	  || cc_mode == 42
	  || cc_mode == 45
	  || cc_mode == 46
	  || cc_mode == 47
	  || cc_mode == 51
	  || cc_mode == 52
	  || cc_mode == 54
	  || cc_mode == 55
	  || cc_mode == 60
	  || cc_mode == 61
	  || cc_mode == 62
	  || cc_mode == 64
	  || cc_mode == 65
	  || cc_mode == 66
	  || cc_mode == 67 || cc_mode == 70 || cc_mode == SPECIAL_VALUE)
	hdy_prefix = DIS_HLS_PREF;
      else
	hdy_prefix = "+";
      /*
         Calculate the year number used in the Islamic calendar.
       */
      (void) doy2date (doy, (days_of_february (year) == 29), &d, &m);
      y = (int) ROUND (((date2num (d, m, year) - 227015L) / 354.36707) + 1.0);
      sprintf (s2, "%s %d", _(hd_text[HD_ISL_NEW_YEARS_DAY].ht_text), y);
      holiday (*init_data, detected, s2, ptr_cc_id,
	       hdy_prefix, doy, 0, year, hd_elems, fday, count);
      if (*init_data)
	*init_data = FALSE;
      if (doy2)
	{
	  /*
	     MuHarram_1 appears twice in YEAR.
	   */
	  check_twice = TRUE;
	  sprintf (s2, "%s %d", _(hd_text[HD_ISL_NEW_YEARS_DAY].ht_text),
		   y + 1);
	  holiday (*init_data, detected, s2, ptr_cc_id, hdy_prefix, doy2, 0,
		   year, hd_elems, fday, count);
	}
      /*
         Manage those holidays last, which can exceed the
         year bounds of a Julian/Gregorian calendar year.
       */
      if (cc_mode != SPECIAL_VALUE)
	for (n = year - 1; n <= year; n++)
	  if (n > 621)
	    {
	      doy = muharram_1 (n, &doy2);
	      if (doy2 && (n == year - 1))
		(void) doy2date (doy2, (days_of_february (n) == 29), &d, &m);
	      else
		(void) doy2date (doy, (days_of_february (n) == 29), &d, &m);
	    LABEL_ih_muharram_1_twice:
	      idate = date2num (d, m, n);
	      /*
	         And manage all ``critical'' holidays.
	       */
	      if (!cc_mode
		  || cc_mode == 2
		  || cc_mode == 4
		  || cc_mode == 8
		  || cc_mode == 14
		  || cc_mode == 26
		  || cc_mode == 27
		  || cc_mode == 28
		  || cc_mode == 35 || cc_mode == 49 || cc_mode == 50)
		{
		  if (!cc_mode || cc_mode == 26)
		    hdy_prefix = DIS_HLS_PREF;
		  else if (cc_mode == 2)
		    hdy_prefix = DIS_HLS_PREF2;
		  else
		    hdy_prefix = "+";
		  if (!cc_mode || cc_mode == 26)
		    j = 1;
		  else
		    j = 2;
		  for (i = 0; i < j; i++)
		    {
		      num2date (idate + 9 + i, &d, &m, &y);
		      if (y == year)
			holiday (*init_data, detected,
				 _(hd_text[HD_ISL_ASHURA].ht_text), ptr_cc_id,
				 hdy_prefix, d, m, year, hd_elems, fday,
				 count);
		    }
		}
	      if ((cc_mode != 3)
		  && (cc_mode != 4)
		  && (cc_mode != 12)
		  && (cc_mode != 20)
		  && (cc_mode != 30)
		  && (cc_mode != 39)
		  && (cc_mode != 42)
		  && (cc_mode != 47)
		  && (cc_mode != 50)
		  && (cc_mode != 51)
		  && (cc_mode != 54)
		  && (cc_mode != 60)
		  && (cc_mode != 61)
		  && (cc_mode != 62)
		  && (cc_mode != 64)
		  && (cc_mode != 65) && (cc_mode != 67) && (cc_mode != 70))
		{
		  if (!cc_mode || cc_mode == 26)
		    hdy_prefix = DIS_HLS_PREF;
		  else if (cc_mode == 36)
		    hdy_prefix = DIS_HLS_PREF2;
		  else
		    hdy_prefix = "+";
		  if (cc_mode == 48)
		    j = 2;
		  else
		    j = 1;
		  for (i = 0; i < j; i++)
		    {
		      num2date (idate + 70 + i, &d, &m, &y);
		      if (y == year)
			holiday (*init_data, detected,
				 _(hd_text[HD_ISL_EID_I_MILAD_UN_NABI].
				   ht_text), ptr_cc_id, hdy_prefix, d, m,
				 year, hd_elems, fday, count);
		    }
		}
	      if (!cc_mode
		  || cc_mode == 1
		  || cc_mode == 10
		  || cc_mode == 25
		  || cc_mode == 29
		  || cc_mode == 33
		  || cc_mode == 35 || cc_mode == 48 || cc_mode == 69)
		{
		  num2date (idate + 203, &d, &m, &y);
		  if (y == year)
		    holiday (*init_data, detected,
			     _(hd_text[HD_ISL_SHAB_E_MIRAJ].ht_text),
			     ptr_cc_id, (cc_mode) ? "+" : DIS_HLS_PREF, d, m,
			     year, hd_elems, fday, count);
		}
	      if (!cc_mode || cc_mode == 6)
		{
		  num2date (idate + 221, &d, &m, &y);
		  if (y == year)
		    holiday (*init_data, detected,
			     _(hd_text[HD_ISL_SHAB_E_BARAT].ht_text),
			     ptr_cc_id, (cc_mode) ? "+" : DIS_HLS_PREF, d, m,
			     year, hd_elems, fday, count);
		}
	      if (cc_mode == 10 || cc_mode == 43 || cc_mode == 61)
		hdy_prefix = "+";
	      else
		hdy_prefix = DIS_HLS_PREF;
	      num2date (idate + 236, &d, &m, &y);
	      if (y == year)
		holiday (*init_data, detected,
			 _(hd_text[HD_ISL_RAMADAN].ht_text), ptr_cc_id,
			 hdy_prefix, d, m, year, hd_elems, fday, count);
	      if (!cc_mode || cc_mode == 6)
		{
		  num2date (idate + 262, &d, &m, &y);
		  if (y == year)
		    holiday (*init_data, detected,
			     _(hd_text[HD_ISL_SHAB_E_QADR].ht_text),
			     ptr_cc_id, (cc_mode) ? "+" : DIS_HLS_PREF, d, m,
			     year, hd_elems, fday, count);
		}
	      if (!cc_mode || cc_mode == 62)
		hdy_prefix = DIS_HLS_PREF;
	      else
		if (cc_mode == 36
		    || cc_mode == 39 || cc_mode == 60 || cc_mode == 70)
		hdy_prefix = DIS_HLS_PREF2;
	      else
		hdy_prefix = "+";
	      if (cc_mode == 18)
		j = 0;
	      else if (cc_mode == 5
		       || cc_mode == 6
		       || cc_mode == 7
		       || cc_mode == 9
		       || cc_mode == 10
		       || cc_mode == 12
		       || cc_mode == 16
		       || cc_mode == 17
		       || cc_mode == 19
		       || cc_mode == 21
		       || cc_mode == 24
		       || cc_mode == 26
		       || cc_mode == 30
		       || cc_mode == 36
		       || cc_mode == 42
		       || cc_mode == 44
		       || cc_mode == 45
		       || cc_mode == 46
		       || cc_mode == 47
		       || cc_mode == 51
		       || cc_mode == 54
		       || cc_mode == 55
		       || cc_mode == 57
		       || cc_mode == 60
		       || cc_mode == 62
		       || cc_mode == 65
		       || cc_mode == 66 || cc_mode == 67 || cc_mode == 70)
		j = 1;
	      else if (cc_mode == 2
		       || cc_mode == 11
		       || cc_mode == 13
		       || cc_mode == 14
		       || cc_mode == 20
		       || cc_mode == 25 || cc_mode == 38 || cc_mode == 62)
		j = 2;
	      else if (cc_mode == 28
		       || cc_mode == 48 || cc_mode == 50 || cc_mode == 69)
		j = 4;
	      else if (cc_mode == 4 || cc_mode == 52)
		j = 7;
	      else
		j = 3;
	      for (i = 0; i < j; i++)
		{
		  num2date (idate + 266 + i, &d, &m, &y);
		  if (y == year)
		    holiday (*init_data, detected,
			     _(hd_text[HD_ISL_EID_AL_FITR].ht_text),
			     ptr_cc_id, hdy_prefix, d, m, year, hd_elems,
			     fday, count);
		}
	      if (!cc_mode || cc_mode == 2 || cc_mode == 15 || cc_mode == 43)
		{
		  num2date (idate + 333, &d, &m, &y);
		  if (y == year)
		    holiday (*init_data, detected,
			     _(hd_text[HD_ISL_WAQF_AL_ARAFAT].ht_text),
			     ptr_cc_id, (cc_mode) ? "+" : DIS_HLS_PREF, d, m,
			     year, hd_elems, fday, count);
		}
	      if (!cc_mode || cc_mode == 26)
		hdy_prefix = DIS_HLS_PREF;
	      else
		if (cc_mode == 36
		    || cc_mode == 39 || cc_mode == 60 || cc_mode == 70)
		hdy_prefix = DIS_HLS_PREF2;
	      else
		hdy_prefix = "+";
	      if (cc_mode == 3
		  || cc_mode == 18
		  || cc_mode == 30
		  || cc_mode == 42
		  || cc_mode == 47 || cc_mode == 51 || cc_mode == 65)
		j = 0;
	      else if (cc_mode == 5
		       || cc_mode == 6
		       || cc_mode == 7
		       || cc_mode == 9
		       || cc_mode == 10
		       || cc_mode == 12
		       || cc_mode == 16
		       || cc_mode == 17
		       || cc_mode == 19
		       || cc_mode == 21
		       || cc_mode == 24
		       || cc_mode == 25
		       || cc_mode == 36
		       || cc_mode == 44
		       || cc_mode == 45
		       || cc_mode == 46
		       || cc_mode == 54
		       || cc_mode == 55
		       || cc_mode == 57
		       || cc_mode == 60
		       || cc_mode == 66 || cc_mode == 67 || cc_mode == 70)
		j = 1;
	      else if (cc_mode == 2
		       || cc_mode == 11
		       || cc_mode == 13
		       || cc_mode == 14
		       || cc_mode == 20 || cc_mode == 38 || cc_mode == 62)
		j = 2;
	      else if (cc_mode == 28
		       || cc_mode == 29
		       || cc_mode == 43
		       || cc_mode == 48 || cc_mode == 50 || cc_mode == 64)
		j = 4;
	      else if (cc_mode == 69)
		j = 5;
	      else if (cc_mode == 4 || cc_mode == 52)
		j = 10;
	      else
		j = 3;
	      for (i = 0; i < j; i++)
		{
		  num2date (idate + 334 + i, &d, &m, &y);
		  if (y == year)
		    holiday (*init_data, detected,
			     _(hd_text[HD_ISL_EID_AL_ADHA].ht_text),
			     ptr_cc_id, hdy_prefix, d, m, year, hd_elems,
			     fday, count);
		}
	      if (cc_mode == 43)
		{
		  /*
		     Manage some specific holidays celebrated in Maldives (MV),
		     which are tied to the Islamic calendar.
		   */
		  num2date (idate + 59, &d, &m, &y);
		  if (y == year)
		    holiday (*init_data, detected,
			     _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
			     ptr_cc_id, "+", d, m, year, hd_elems, fday,
			     count);
		  num2date (idate + 177, &d, &m, &y);
		  if (y == year)
		    holiday (*init_data, detected,
			     _(hd_text[HD_HURAVEE_DAY].ht_text), ptr_cc_id,
			     "+", d, m, year, hd_elems, fday, count);
		  num2date (idate + 207, &d, &m, &y);
		  if (y == year)
		    holiday (*init_data, detected,
			     _(hd_text[HD_MARTYRS_DAY].ht_text), ptr_cc_id,
			     "+", d, m, year, hd_elems, fday, count);
		}
	      if (!cc_mode)
		{
		  num2date (idate + 15, &d, &m, &y);
		  if (y == year)
		    holiday (*init_data, detected,
			     _(hd_text[HD_ISL_IMAMAT_DAY].ht_text), ptr_cc_id,
			     DIS_HLS_PREF, d, m, year, hd_elems, fday, count);
		  num2date (idate + 170, &d, &m, &y);
		  if (y == year)
		    holiday (*init_data, detected,
			     _(hd_text[HD_ISL_AGA_KHANS_BIRTHDAY].ht_text),
			     ptr_cc_id, DIS_HLS_PREF, d, m, year, hd_elems,
			     fday, count);
		  num2date (idate + 252, &d, &m, &y);
		  if (y == year)
		    holiday (*init_data, detected,
			     _(hd_text[HD_ISL_NUZUL_AL_QURAN].ht_text),
			     ptr_cc_id, DIS_HLS_PREF, d, m, year, hd_elems,
			     fday, count);
		  num2date (idate + 265, &d, &m, &y);
		  LOOP
		  {
		    if (weekday_of_date (d, m, y) == 5)
		      break;
		    else
		      (void) prev_date (&d, &m, &y);
		  }
		  if (y == year)
		    holiday (*init_data, detected,
			     _(hd_text[HD_ISL_QUDS_DAY].ht_text), ptr_cc_id,
			     DIS_HLS_PREF, d, m, year, hd_elems, fday, count);
		  num2date (idate + 342, &d, &m, &y);
		  if (y == year)
		    holiday (*init_data, detected,
			     _(hd_text[HD_ISL_GHADIR].ht_text), ptr_cc_id,
			     DIS_HLS_PREF, d, m, year, hd_elems, fday, count);
		}
	      if (check_twice && (n == year))
		{
		  check_twice = FALSE;
		  (void) doy2date (doy2, (days_of_february (n) == 29), &d,
				   &m);
		  goto LABEL_ih_muharram_1_twice;
		}
	    }
    }
}



void
islamic_mth (init_data, detected, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all Islamic civil-calendar based months since AD 622.
     Note that this function uses the arithmetical lunar Islamic ***CIVIL***
     calendar.  This kind of calendar is just a rough estimate of the
     religious Islamic lunar calendar, which is calculated astronomically.
     So don't be surprised if some month dates differ from the religious
     Islamic calendar about +/-1...2 days in some years.
*/
{
  if (year > 621)
    {
      auto Ulint idate;
      register int i;
      register int j;
      register int n;
      register int doy;
      auto int doy2;
      auto int d2;
      auto int m2;
      auto int d;
      auto int m;
      auto int y;
      auto Bool check_twice = FALSE;


      ptr_cc_id = _("Isl");
      (void) muharram_1 (year, &doy2);
      if (doy2)
	/*
	   MuHarram_1 appears twice in YEAR.
	 */
	check_twice = TRUE;
      for (n = year - 1; n <= year; n++)
	if (n > 621)
	  {
	    doy = muharram_1 (n, &doy2);
	    if (doy2 && (n == year - 1))
	      j = doy2;
	    else
	      j = doy;
	    (void) doy2date (j, (days_of_february (n) == 29), &d, &m);
	  LABEL_im_muharram_1_twice:
	    idate = date2num (d, m, n);
	    /*
	       And manage all Islamic civil-calendar months.
	     */
	    for (i = MONTH_MIN; i <= MONTH_MAX; i++)
	      {
		num2date (idate, &d, &m, &y);
		if (y == year)
		  {
		    /*
		       Calculate the year number used in the Islamic calendar.
		     */
		    (void) doy2date (j, (days_of_february (year) == 29), &d2,
				     &m2);
		    y =
		      (int)
		      ROUND (((date2num (d2, m2, n) - 227015L) / 354.36707) +
			     1.0);
		    ocs_monthtext (init_data, detected, ptr_cc_id, d, m, year,
				   FALSE, i, y, SPECIAL_VALUE, hd_elems, fday,
				   count);
		  }
		idate += ((i & 1) ? 30 : 29);
	      }
	    if (check_twice && (n == year))
	      {
		check_twice = FALSE;
		(void) doy2date (doy2, (days_of_february (n) == 29), &d, &m);
		goto LABEL_im_muharram_1_twice;
	      }
	  }
    }
}



void
japanese_hdy (init_data, detected, year, hd_elems, fday, count, cc,
	      is_any_meridian)
     Bool *init_data;
     const Bool detected;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
     const char *cc;
     const Bool is_any_meridian;
/*!
   Manages all Japanese calendar based holidays since AD 1645.
     Earlier years are not respected, because the rules of the Japanese
     calendar have changed too many times during these former years.
     The function results have to be interpreted with caution, because
     of the inaccuratenesses of the Moon phase and the Sun longitude
     calculation function used.  The rules to calculate the Japanese
     calendar are identical the rules to calculate the Chinese calendar,
     except that since AD 1888 they use the longitude 135 degrees east
     for computations.
   See the `chinese_hdy()' function for further information.
*/
{
  if (year > 1644)
    {
      auto Ulint conjunction_vector[MONTH_MAX + 4];
      auto Ulint *ptr_conjunction_vector = conjunction_vector;
      auto Ulint zq;
      register int cj;
      register int lm;
      register int cc_mode;
      register int hour;
      register int min;
      register int extra = 0;
      auto int ssd1 = 0;
      auto int ssd2 = 0;
      register int i;
      register int j;
      register int k;
      auto int d;
      auto int m;
      auto int y = year;
      auto Bool is_checked = FALSE;
      auto Bool check_twice = FALSE;


      if (cc == (char *) NULL)
	cc_mode = 0;
      else if (!strcmp (cc, "KR") || !strcmp (cc, "KP") || !strcmp (cc, "TW"))
	cc_mode = 1;
      else if (!strcmp (cc, "JP"))
	cc_mode = 2;
      else if (!strcmp (cc, HD_MCNY))
	cc_mode = SPECIAL_VALUE;
      else
	abort ();		/* Error, invalid case! */
      if (!cc_mode || cc_mode == SPECIAL_VALUE)
	ptr_cc_id = _("Jap");
      if (is_any_meridian)
	{
	  /*
	     Astronomical calculations are done for a line at a definite
	     meridian expressed as a time value.  This time value is given
	     by the global `time_hour_offset' variable and by the global
	     `time_min_offset' variable.  If HOUR and MIN are set to zero,
	     calculations are made for Universal Time (UTC/GMT).  If HOUR
	     and MIN have a positive sign, UTC/GMT calculations are made
	     for meridians East of Greenwich, otherwise for meridians
	     West of Greenwich.
	   */
	  hour = time_hour_offset;
	  min = time_min_offset;
	}
      else
	{
	  /*
	     Astronomical calculations are based on China/Japan local time.
	   */
	  if (year < 1888)
	    {
	      /*
	         Meridian Beijing (116 25' degrees East), UTC/GMT time difference.
	       */
	      hour = 7;
	      min = 45;
	    }
	  else
	    {
	      /*
	         UTC/GMT-9 for the meridian 135 degrees East.
	       */
	      hour = 9;
	      min = 0;
	    }
	}
      if (cc_mode != SPECIAL_VALUE)
	{
	  /*
	     Manage the major solar term/Zhong-Qi related festivals first.
	   */
	  if (equinox_solstice (NH_VE, &d, &m, &y, hour, min) !=
	      SPECIAL_VALUE)
	    {
	      holiday (*init_data, detected,
		       _(hd_text[HD_EQUINOX_DAY].ht_text), ptr_cc_id,
		       (cc_mode) ? "+" : DIS_HLS_PREF, d, m, y, hd_elems,
		       fday, count);
	      if (*init_data)
		*init_data = FALSE;
	    }
	  else
	    y = year;
	  if (equinox_solstice (NH_AE, &d, &m, &y, hour, min) !=
	      SPECIAL_VALUE)
	    {
	      holiday (*init_data, detected,
		       _(hd_text[HD_EQUINOX_DAY].ht_text), ptr_cc_id,
		       (cc_mode) ? "+" : DIS_HLS_PREF, d, m, y, hd_elems,
		       fday, count);
	      if (*init_data)
		*init_data = FALSE;
	    }
	  else
	    y = year;
	  if (equinox_solstice (NH_WS, &d, &m, &y, hour, min) !=
	      SPECIAL_VALUE)
	    {
	      holiday (*init_data, detected,
		       _(hd_text[HD_SOLSTICE_DAY].ht_text), ptr_cc_id,
		       (cc_mode) ? "+" : DIS_HLS_PREF, d, m, y, hd_elems,
		       fday, count);
	      if (*init_data)
		*init_data = FALSE;
	    }
	  else
	    y = year;
	  /*
	     And manage the minor solar term/Jie-Qi related festivals next.
	   */
	  if (sun_longitude (315.0, &d, &m, &y, hour, min, FALSE) !=
	      SPECIAL_VALUE)
	    {
	      (void) prev_date (&d, &m, &y);
	      if (y == year)
		{
		  holiday (*init_data, detected,
			   _(hd_text[HD_SEASON_CHANGE].ht_text), ptr_cc_id,
			   DIS_HLS_PREF, d, m, y, hd_elems, fday, count);
		  if (*init_data)
		    *init_data = FALSE;
		}
	    }
	  y = year;
	}
    LABEL_japanese_new_year_twice:
      /*
         And now the lunar based holidays, so check for a possible leap month.
       */
      lm =
	find_chinese_leap_month (ptr_conjunction_vector, &zq, y, hour, min);
      if (lm == SPECIAL_VALUE)
	{
	  /*
	     Error, cannot compute the lunisolar based calendar correctly.
	   */
	  if (warning_level >= 0)
	    {
	      sprintf (s2,
		       _
		       ("Cannot calculate lunisolar calendar for %d correctly"),
		       y);
	      print_text (stderr, s2);
	      if (warning_level >= WARN_LVL_MAX)
		my_error (ERR_INTERNAL_C_FUNC_FAILURE, __FILE__,
			  ((long) __LINE__) - 15L,
			  "find_chinese_leap_month()::lm!=", lm);
	    }
	}
      else
	{
	  /*
	     Now, since we know the leap month in the year YEAR, we can
	     calculate the date of the Japanese_New_Year/Japanese calendar
	     month 1 correctly.  The Japanese_New_Year is on the date of the
	     second New Moon ***after*** the date of the winter solstice
	     (which is always in the 11th month of the Japanese calendar)
	     of the previous YEAR, but only if there is NO leap month 11
	     or 12 (this case is marked by `lm==-11|-12'), otherwise it is
	     on the date of the third New Moon ***after*** the date of the
	     winter solstice of the previous YEAR.
	   */
	  cj = 0;
	  if (conjunction_vector[cj] == zq)
	    cj++;
	  if (lm < 0)
	    cj += 2;
	  else
	    cj++;
	  if (cc_mode != SPECIAL_VALUE)
	    {
	      /*
	         Detect the date of the first day of the Japanese calendar
	         month 12 of the previous YEAR.
	       */
	      if (lm == -MONTH_MAX)
		num2date (conjunction_vector[cj - 2] + 12, &d, &m, &y);
	      else
		num2date (conjunction_vector[cj - 1] + 12, &d, &m, &y);
	      if (y == year)
		{
		  i = day_of_year (d, m, y);
		  if ((i != ssd1) && (i != ssd2))
		    {
		      ssd1 = i;
		      holiday (*init_data, detected,
			       _(hd_text[HD_SOOT_SWEEPING_DAY].ht_text),
			       ptr_cc_id, DIS_HLS_PREF, d, m, y, hd_elems,
			       fday, count);
		      if (*init_data)
			*init_data = FALSE;
		    }
		}
	    }
	  /*
	     Detect the date of the last day of the Japanese calendar month,
	     which is prior the first day of the Japanese calendar month 1.
	   */
	  num2date (conjunction_vector[cj] - 1, &d, &m, &y);
	  if ((y == year) && (cc_mode != SPECIAL_VALUE))
	    {
	      holiday (*init_data, detected,
		       _(hd_text[HD_JAPANESE_NEW_YEARS_EVE].ht_text),
		       ptr_cc_id, DIS_HLS_PREF, d, m, y, hd_elems, fday,
		       count);
	      if (*init_data)
		*init_data = FALSE;
	    }
	  /*
	     Detect the date of the first day of the Japanese calendar month 1.
	   */
	  (void) next_date (&d, &m, &y);
	  /*
	     Detect whether it is necessary to respect the holidays of next
	     YEAR.  This must be always done if we creating dates according
	     to the proleptic Julian calendar and the Japanese_New_Year
	     holiday either started in the previous YEAR or started early
	     in the actual YEAR.
	   */
	  if (!is_checked
	      && ((y != year) || ((year <= greg->year) && (year >= 2700))))
	    is_checked = check_twice = TRUE;
	  if (y == year)
	    {
	      /*
	         Calculate the year number used in the Japanese calendar,
	         based on the first historical record of the 60-year cycle
	         from 660 BCE.
	       */
	      k = y + 660 + extra;
	      sprintf (s2, "%s %d-%d",
		       _(hd_text[HD_JAPANESE_NEW_YEARS_DAY].ht_text), k,
		       (lm < 0) ? 0 : lm);
	      holiday (*init_data, detected, s2, ptr_cc_id,
		       (cc_mode > 0) ? "+" : DIS_HLS_PREF, d, m, y, hd_elems,
		       fday, count);
	      if (*init_data)
		*init_data = FALSE;
	      if (!cc_mode || cc_mode == SPECIAL_VALUE)
		{
		  /*
		     Compute the sexagesimal cycle data based on the year number
		     used in the Chinese calendar, which is based on the first
		     historical record of the 60-year cycle from 08-Mar-2637 BCE.
		   */
		  k = y + 2637 + extra;
		  i = ((k - 1) / 60) + 1;
		  j = ((k - 1) % 60) + 1;
		  sprintf (s2, "%s %d/%02d-%02d %s-%s",
			   _(hd_text[HD_CHINESE_CYCLE].ht_text), i, j,
			   (lm < 0) ? 0 : lm, chi_stem[((j - 1) % 10)],
			   _(chi_branch[((j - 1) % 12)]));
		  holiday (*init_data, detected, s2, ptr_cc_id, DIS_HLS_PREF,
			   d, m, y, hd_elems, fday, count);
		}
	    }
	  if (cc_mode != SPECIAL_VALUE)
	    {
	      if (!cc_mode)
		j = 6;
	      else
		j = 2;
	      for (i = 0; i < j; i++)
		{
		  (void) next_date (&d, &m, &y);
		  if (y == year)
		    holiday (*init_data, detected,
			     _(hd_text[HD_JAPANESE_NEW_YEARS_DAY].ht_text),
			     ptr_cc_id, (cc_mode) ? "+" : DIS_HLS_PREF, d, m,
			     y, hd_elems, fday, count);
		}
	      /*
	         Detect the date of the first day of the Japanese calendar
	         month 3.
	       */
	      if (lm == 1 || lm == 2)
		cj++;
	      cj += 2;
	      num2date (conjunction_vector[cj] + 2, &d, &m, &y);
	      if (y == year)
		{
		  holiday (*init_data, detected,
			   _(hd_text[HD_MUSIC_WATER_BANQUET].ht_text),
			   ptr_cc_id, DIS_HLS_PREF, d, m, y, hd_elems, fday,
			   count);
		  if (*init_data)
		    *init_data = FALSE;
		}
	      /*
	         Detect the date of the first day of the Japanese calendar
	         month 4.
	       */
	      if (lm == 3)
		cj++;
	      num2date (conjunction_vector[++cj] + 7, &d, &m, &y);
	      if (y == year)
		{
		  holiday (*init_data, detected,
			   _(hd_text[HD_BUDDHAS_BIRTHDAY].ht_text), ptr_cc_id,
			   DIS_HLS_PREF, d, m, y, hd_elems, fday, count);
		  if (*init_data)
		    *init_data = FALSE;
		}
	      /*
	         Detect the date of the first day of the Japanese calendar
	         month 5.
	       */
	      if (lm == 4)
		cj++;
	      num2date (conjunction_vector[++cj] + 4, &d, &m, &y);
	      if (y == year)
		{
		  holiday (*init_data, detected,
			   _(hd_text[HD_CHILDRENS_DAY].ht_text), ptr_cc_id,
			   DIS_HLS_PREF, d, m, y, hd_elems, fday, count);
		  if (*init_data)
		    *init_data = FALSE;
		}
	      /*
	         Detect the date of the first day of the Japanese calendar
	         month 7.
	       */
	      if (lm == 5 || lm == 6)
		cj++;
	      cj += 2;
	      num2date (conjunction_vector[cj] + 6, &d, &m, &y);
	      if (y == year)
		{
		  holiday (*init_data, detected,
			   _(hd_text[HD_STAR_FESTIVAL].ht_text), ptr_cc_id,
			   DIS_HLS_PREF, d, m, y, hd_elems, fday, count);
		  if (*init_data)
		    *init_data = FALSE;
		}
	      num2date (conjunction_vector[cj] + 14, &d, &m, &y);
	      for (i = 0; i < 3; i++)
		{
		  if (y == year)
		    {
		      holiday (*init_data, detected,
			       _(hd_text[HD_FEAST_OF_FORTUNE].ht_text),
			       ptr_cc_id, DIS_HLS_PREF, d, m, y, hd_elems,
			       fday, count);
		      if (*init_data)
			*init_data = FALSE;
		      (void) next_date (&d, &m, &y);
		    }
		}
	      /*
	         Detect the date of the first day of the Japanese calendar
	         month 8.
	       */
	      if (lm == 7)
		cj++;
	      num2date (conjunction_vector[++cj] + 14, &d, &m, &y);
	      if (y == year)
		{
		  holiday (*init_data, detected,
			   _(hd_text[HD_MID_AUTUMN_FESTIVAL].ht_text),
			   ptr_cc_id, (cc_mode) ? "+" : DIS_HLS_PREF, d, m, y,
			   hd_elems, fday, count);
		  if (*init_data)
		    *init_data = FALSE;
		}
	      /*
	         Detect the date of the first day of the Japanese calendar
	         month 9.
	       */
	      if (lm == 8)
		cj++;
	      num2date (conjunction_vector[++cj] + 8, &d, &m, &y);
	      if (y == year)
		{
		  holiday (*init_data, detected,
			   _(hd_text[HD_CHRYSANTHENUM_DAY].ht_text),
			   ptr_cc_id, DIS_HLS_PREF, d, m, y, hd_elems, fday,
			   count);
		  if (*init_data)
		    *init_data = FALSE;
		}
	      /*
	         Detect the date of the first day of the Japanese calendar
	         month 10.
	       */
	      if (lm == 9)
		cj++;
	      num2date (conjunction_vector[++cj] + 5, &d, &m, &y);
	      if (y == year)
		{
		  holiday (*init_data, detected,
			   _(hd_text[HD_BODHIDHARMAS_BIRTHDAY].ht_text),
			   ptr_cc_id, DIS_HLS_PREF, d, m, y, hd_elems, fday,
			   count);
		  if (*init_data)
		    *init_data = FALSE;
		}
	      /*
	         Detect the date of the first day of the Japanese calendar
	         month 12.
	       */
	      if (lm == 10 || lm == 11)
		cj++;
	      cj += 2;
	      num2date (conjunction_vector[cj] + 12, &d, &m, &y);
	      if (y == year)
		{
		  i = day_of_year (d, m, y);
		  if ((i != ssd1) && (i != ssd2))
		    {
		      ssd2 = i;
		      holiday (*init_data, detected,
			       _(hd_text[HD_SOOT_SWEEPING_DAY].ht_text),
			       ptr_cc_id, DIS_HLS_PREF, d, m, y, hd_elems,
			       fday, count);
		      if (*init_data)
			*init_data = FALSE;
		    }
		}
	    }
	  if (check_twice)
	    {
	      check_twice = FALSE;
	      y = year + 1;
	      extra++;
	      goto LABEL_japanese_new_year_twice;
	    }
	}
    }
}



void
multicultural_new_year_hdy (init_data, detected, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all Multicultural New_Year's_Day holidays, which are majorly
     based on calendar systems different the Julian/Gregorian calendar system.
*/
{
  auto double x;
  auto Ulint mjd;
  auto int d;
  auto int m;
  auto int y;


  bahai_hdy (init_data, detected, year, hd_elems, fday, count, HD_MCNY);
  celtic_hdy (init_data, detected, year, hd_elems, fday, count, HD_MCNY);
  chinese_hdy (init_data, detected, year, hd_elems, fday, count, HD_MCNY,
	       FALSE);
  hebrew_hdy (init_data, detected, year, hd_elems, fday, count, HD_MCNY);
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, HD_MCNY);
  japanese_hdy (init_data, detected, year, hd_elems, fday, count, HD_MCNY,
		FALSE);
  orthodox_hdy (init_data, detected, 0, year, hd_elems, fday, count, HD_MCNY,
		TRUE);
  orthodox_hdy (init_data, detected, 0, year, hd_elems, fday, count, HD_MCNY,
		FALSE);
  persian_hdy (init_data, detected, year, hd_elems, fday, count, HD_MCNY);
  if (year > 1791)
    {
      /*
         Calculate the year number used in the French Revolutionary
         (Republican) calendar, based on the date of the autumnal
         equinox as it occurs in Paris, FR.
       */
      ptr_cc_id = _("FRR");
      y = year;
      mjd = (Ulint) equinox_solstice (NH_AE, &d, &m, &y, 0, 9);
      if (mjd != (Ulint) SPECIAL_VALUE)
	{
	  sprintf (s2, "%s %d", _(hd_text[HD_FRENCH_REVOLUTIONARY].ht_text),
		   year - 1791);
	  holiday (*init_data, detected, s2, ptr_cc_id, DIS_HLS_PREF, d, m,
		   year, hd_elems, fday, count);
	}
    }
  if (year > 1887)
    {
      /*
         Calculate the year number used in the Thai ***CIVIL***
         (western oriented) calendar based on the Buddhist Era.
       */
      ptr_cc_id = "TH";
      if (year > 1939)
	m = MONTH_MIN;
      else
	m = 4;
      if (year <= greg->year)
	{
	  if (m == MONTH_MIN)
	    {
	      mjd = date2num (DAY_MIN, m, year + 1);
	      mjd -= (((year + 1) / 100) - ((year + 1) / 400) - 2);
	      num2date (mjd, &d, &m, &y);
	      d = day_of_year (d, m, y);
	      m = year + 1;
	    }
	  else
	    {
	      d = day_of_year (DAY_MIN, m, year);
	      d -= ((year / 100) - (year / 400) - 2);
	      m = year;
	    }
	}
      else
	{
	  d = day_of_year (DAY_MIN, m, year);
	  m = year;
	}
      sprintf (s2, "%s %d", _(hd_text[HD_THAI_NEW_YEARS_DAY].ht_text),
	       m + 543);
      holiday (*init_data, detected, s2, ptr_cc_id, DIS_HLS_PREF, d, 0, year,
	       hd_elems, fday, count);
    }
  if (year > 1956)
    {
      /*
         Calculate the year number used in the Indian **CIVIL** (reformed)
         calendar corresponding to Caitra 1.
       */
      ptr_cc_id = "IN";
      sprintf (s2, "%s %d", _(hd_text[HD_INDIAN_NEW_YEARS_DAY].ht_text),
	       year - 78);
      d = 22 - (days_of_february (year) == 29);
      if (year <= greg->year)
	d -= 13;
      holiday (*init_data, detected, s2, ptr_cc_id,
	       DIS_HLS_PREF, d, 3, year, hd_elems, fday, count);
    }
  if (year > 551)
    {
      /*
         Calculate the year number used in the Old-Armenic calendar,
         based on the Epoch AD 11-Jul-0552.
       */
      ptr_cc_id = _("AMO");
      mjd = date2num (DAY_MIN, MONTH_MIN, year) - 201081L;
      x = mjd / (double) DAY_LAST;
      y = (int) x + 1;
      x = ROUND ((1.0 - (x - (Ulint) x)) * (double) DAY_LAST);
      sprintf (s2, "%s %d", _(hd_text[HD_OLD_ARMENIC_NEW_YEARS_DAY].ht_text),
	       y);
      holiday (*init_data, detected, s2, ptr_cc_id, DIS_HLS_PREF, (int) x, 0,
	       year, hd_elems, fday, count);
      if ((days_of_february (year) == 29) && ((int) x == 1))
	{
	  sprintf (s2, "%s %d",
		   _(hd_text[HD_OLD_ARMENIC_NEW_YEARS_DAY].ht_text), y + 1);
	  holiday (*init_data, detected, s2, ptr_cc_id, DIS_HLS_PREF,
		   DAY_LAST + 1, 0, year, hd_elems, fday, count);
	}
    }
  /*
     Calculate the year number and the Sothisperiod used in the Old-Egyptic
     calendar, based on the Epoch 19-Jul-4241 BCE.
   */
  ptr_cc_id = _("EGO");
  mjd = date2num (DAY_MIN, MONTH_MIN, year) + MIN_BCE_TO_1_CE - 172235L;
  m = ((int) (mjd / 533256L)) + 1;
  x = mjd / (double) DAY_LAST;
  y = (int) x + 1;
  x = ROUND ((1.0 - (x - (Ulint) x)) * (double) DAY_LAST);
  sprintf (s2, "%s %d-%d", _(hd_text[HD_OLD_EGYPTIC_NEW_YEARS_DAY].ht_text),
	   y, m);
  holiday (*init_data, detected, s2, ptr_cc_id, DIS_HLS_PREF, (int) x, 0,
	   year, hd_elems, fday, count);
  if ((days_of_february (year) == 29) && ((int) x == 1))
    {
      sprintf (s2, "%s %d-%d",
	       _(hd_text[HD_OLD_EGYPTIC_NEW_YEARS_DAY].ht_text), y + 1, m);
      holiday (*init_data, detected, s2, ptr_cc_id, DIS_HLS_PREF,
	       DAY_LAST + 1, 0, year, hd_elems, fday, count);
    }
  /*
     Calculate the year number used in the Ethiopic calendar.
   */
  ptr_cc_id = "ET";
  d = day_of_year (29, 8, year)
    + ((year & 3) == 3) + julian_gregorian_diff (29, 8, year);
  sprintf (s2, "%s %d", _(hd_text[HD_ETHIOPIC_NEW_YEARS_DAY].ht_text),
	   year - 7);
  holiday (*init_data, detected, s2, ptr_cc_id, DIS_HLS_PREF, d, 0, year,
	   hd_elems, fday, count);
  if (year > 283)
    {
      /*
         Calculate the year number used in the Coptic calendar.
       */
      ptr_cc_id = "EG";
      sprintf (s2, "%s %d", _(hd_text[HD_COPTIC_NEW_YEARS_DAY].ht_text),
	       year - 283);
      holiday (*init_data, detected, s2, ptr_cc_id, DIS_HLS_PREF, d, 0, year,
	       hd_elems, fday, count);
    }
}



void
old_egyptic_mth (init_data, detected, year, hd_elems, fday, count, do_egyptic)
     Bool *init_data;
     const Bool detected;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
     const Bool do_egyptic;
/*!
   Manages all Old-Armenic/Old-Egyptic calendar based months since
     AD 11-Jul-0552 (Old-Armenic).
*/
{
  if (do_egyptic || (!do_egyptic && (year > 551)))
    {
      auto double mjd;
      auto Ulint edate;
      register int sothisperiod;
      register int em;
      register int ey;
      register int i;
      register int n;
      auto int d;
      auto int m;
      auto int y;
      auto Bool is_special_case = FALSE;


      if (do_egyptic)
	ptr_cc_id = _("EGO");
      else
	ptr_cc_id = _("AMO");
      for (n = year - 1; n <= year; n++)
	if (do_egyptic || (!do_egyptic && (n > 551)))
	  {
	    /*
	       Detect the Old-Armenian/Old-Egyptian_New_Year's_Day date and
	       calculate the year number used in the Old-Armenic calendar
	       -- based on the Epoch AD 11-Jul-0552 -- respectively,
	       calculate the year number used in the Old-Egyptic calendar
	       -- based on the Epoch 19-Jul-4241 BCE --.
	     */
	    if (do_egyptic)
	      {
		mjd =
		  date2num (DAY_MIN, MONTH_MIN,
			    n) + MIN_BCE_TO_1_CE - 172235L;
		sothisperiod = ((int) (mjd / 533256L)) + 1;
	      }
	    else
	      {
		mjd = date2num (DAY_MIN, MONTH_MIN, n) - 201081L;
		sothisperiod = SPECIAL_VALUE;
	      }
	    mjd = mjd / (double) DAY_LAST;
	    ey = (int) mjd + 1;
	    mjd = ROUND ((1.0 - (mjd - (Ulint) mjd)) * (double) DAY_LAST);
	    if ((n == year)
		&& (days_of_february (n - 1) == 29)
		&& ((int) mjd == DAY_LAST))
	      {
		is_special_case = TRUE;
		em = MONTH_MIN;
		ey--;
		mjd = 30.0;
	      }
	    else
	      em = 0;
	    (void) doy2date ((int) mjd, (days_of_february (n) == 29), &d, &m);
	    edate = date2num (d, m, n);
	    /*
	       And manage all Old-Armenic/Old-Egyptic calendar months.
	     */
	    for (i = 0; i <= MONTH_MAX; em++, i++)
	      {
		num2date (edate, &d, &m, &y);
		if (y == year)
		  {
		    if (is_special_case)
		      {
			if (i == MONTH_MAX - 1)
			  edate -= 25;
			else if (i == MONTH_MAX)
			  {
			    em = 0;
			    ey++;
			  }
		      }
		    ocs_monthtext (init_data, detected, ptr_cc_id, d, m, year,
				   (em == MONTH_MAX),
				   (em == MONTH_MAX) ? em : em + 1, ey,
				   sothisperiod, hd_elems, fday, count);
		  }
		edate += 30;
	      }
	    if ((n == year)
		&& (days_of_february (n) == 29) && ((int) mjd == 1))
	      ocs_monthtext (init_data, detected, ptr_cc_id, DAY_LAST + 1, 0,
			     year, FALSE, MONTH_MIN, ey + 1, sothisperiod,
			     hd_elems, fday, count);
	  }
    }
}



void
orthodox_hdy (init_data, detected, easter, year, hd_elems, fday, count, cc,
	      is_new_calendar)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
     const char *cc;
     const Bool is_new_calendar;
/*!
   Manages all Orthodox Christian Eastern churches calendar based holidays.
     If `is_new_calendar' is set to TRUE, the new calendar holidays are
     computed, otherwise the old calendar holidays.
*/
{
  register int i = year;
  register int cc_mode;
  auto int greg_diff;
  auto int d;
  auto int m;
  auto int y;


  if (cc == (char *) NULL)
    cc_mode = 0;
  else if (!strcmp (cc, HD_MCNY))
    cc_mode = SPECIAL_VALUE;
  else
    abort ();			/* Error, invalid case! */
  if (is_new_calendar)
    ptr_cc_id = _("OxN");
  else
    ptr_cc_id = _("OxO");
  /*
     Process the holiday data.
   */
  if (cc_mode == SPECIAL_VALUE)
    {
      d = DAY_MIN;
      if (!is_new_calendar)
	d += julian_gregorian_diff (d, MONTH_MIN, year);
      else if (year <= greg->year)
	{
	  greg_diff = (year / 100) - (year / 400) - 2;
	  if (greg_diff < 0)
	    {
	      auto Ulint mjd;


	      i = y = year + 1;
	      greg_diff = (y / 100) - (y / 400) - 2;
	      mjd = date2num (d, MONTH_MIN, y);
	      num2date (mjd + greg_diff, &d, &m, &y);
	      d = day_of_year (d, m, y);
	    }
	  else
	    d += greg_diff;
	}
      sprintf (s2, "%s %d", _(hd_text[HD_ORTHODOX_NEW_YEARS_DAY].ht_text), i);
      holiday (*init_data, detected, s2, ptr_cc_id,
	       DIS_HLS_PREF, d, 0, year, hd_elems, fday, count);
      if (*init_data)
	*init_data = FALSE;
    }
  else
    {
      easter = orthodox_easter (year, &greg_diff, 1924, 3, 10, 22);
      holiday (*init_data, detected, _(hd_text[HD_ASH_MONDAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, easter - 48, 0, year, hd_elems, fday,
	       count);
      if (*init_data)
	*init_data = FALSE;
      holiday (*init_data, detected, _(hd_text[HD_PALM_SUNDAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, easter - DAY_MAX, 0, year, hd_elems,
	       fday, count);
      holiday (*init_data, detected, _(hd_text[HD_MAUNDY_THURSDAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, easter - 3, 0, year, hd_elems, fday,
	       count);
      holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, easter - 2, 0, year, hd_elems, fday,
	       count);
      holiday (*init_data, detected, _(hd_text[HD_GOOD_SATURDAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, easter - 1, 0, year, hd_elems, fday,
	       count);
      holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, easter, 0, year, hd_elems, fday,
	       count);
      holiday (*init_data, detected,
	       _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text), ptr_cc_id,
	       DIS_HLS_PREF, easter + 39, 0, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_PENTECOST].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, easter + 49, 0, year, hd_elems, fday,
	       count);
      d = DAY_MIN;
      if (!is_new_calendar && (year > 1924))
	d += greg_diff;
      holiday (*init_data, detected,
	       _(hd_text[HD_ORTHODOX_NEW_YEARS_DAY].ht_text), ptr_cc_id,
	       DIS_HLS_PREF, d, 0, year, hd_elems, fday, count);
      d = 6;
      if (!is_new_calendar && (year > 1924))
	d += greg_diff;
      holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, d, 0, year, hd_elems, fday, count);
      d = 14;
      if (!is_new_calendar && (year > 1924))
	d += greg_diff;
      holiday (*init_data, detected,
	       _(hd_text[HD_JESUS_CIRCUMCISION].ht_text), ptr_cc_id,
	       DIS_HLS_PREF, d, 0, year, hd_elems, fday, count);
      d = day_of_year (2, 2, year);
      if (!is_new_calendar && (year > 1924))
	d += greg_diff;
      holiday (*init_data, detected, _(hd_text[HD_MARYS_CANDLEMAS].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, d, 0, year, hd_elems, fday, count);
      d = day_of_year (25, 3, year);
      if (!is_new_calendar)
	d += greg_diff;
      holiday (*init_data, detected,
	       _(hd_text[HD_MARYS_ANNUNCIATION_DAY].ht_text), ptr_cc_id,
	       DIS_HLS_PREF, d, 0, year, hd_elems, fday, count);
      d = day_of_year (6, 8, year);
      if (!is_new_calendar)
	d += greg_diff;
      holiday (*init_data, detected,
	       _(hd_text[HD_TRANSFIGURATION_DAY].ht_text), ptr_cc_id,
	       DIS_HLS_PREF, d, 0, year, hd_elems, fday, count);
      d = day_of_year (15, 8, year);
      if (!is_new_calendar)
	d += greg_diff;
      holiday (*init_data, detected,
	       _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text), ptr_cc_id,
	       DIS_HLS_PREF, d, 0, year, hd_elems, fday, count);
      d = day_of_year (8, 9, year);
      if (!is_new_calendar)
	d += greg_diff;
      holiday (*init_data, detected, _(hd_text[HD_MARYS_NATIVITY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, d, 0, year, hd_elems, fday, count);
      d = day_of_year (14, 9, year);
      if (!is_new_calendar)
	d += greg_diff;
      holiday (*init_data, detected, _(hd_text[HD_CROSS_RAISING_DAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, d, 0, year, hd_elems, fday, count);
      if (is_new_calendar)
	{
	  holiday (*init_data, detected,
		   _(hd_text[HD_ALL_SAINTS_DAY].ht_text), ptr_cc_id,
		   DIS_HLS_PREF, DAY_MIN, 11, year, hd_elems, fday, count);
	  holiday (*init_data, detected, _(hd_text[HD_ALL_SOULS_DAY].ht_text),
		   ptr_cc_id, DIS_HLS_PREF, 2, 11, year, hd_elems, fday,
		   count);
	}
      d = 24;
      m = MONTH_MAX;
      if (!is_new_calendar && (year != 1924))
	{
	  y = year - 1;
	  for (i = 0; i < greg_diff; i++)
	    (void) next_date (&d, &m, &y);
	}
      if (is_new_calendar || (!is_new_calendar && (year != 1924)))
	holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
		 ptr_cc_id, DIS_HLS_PREF, d, m, year, hd_elems, fday, count);
      d = 25;
      m = MONTH_MAX;
      if (!is_new_calendar && (year != 1924))
	{
	  y = year - 1;
	  for (i = 0; i < greg_diff; i++)
	    (void) next_date (&d, &m, &y);
	}
      if (is_new_calendar || (!is_new_calendar && (year != 1924)))
	holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
		 ptr_cc_id, DIS_HLS_PREF, d, m, year, hd_elems, fday, count);
    }
}



void
persian_hdy (init_data, detected, year, hd_elems, fday, count, cc)
     Bool *init_data;
     const Bool detected;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
     const char *cc;
/*!
   Manages all Persian ``Jalaali'' calendar based holidays since AD 622.
     Note that this function calculates the solar ``Jalaali'' calendar
     astronomically and not arithmetically, so necessary calendar reformations
     which must apply in the furture for the arithmetical solution are avoided.
*/
{
  if (year > 621)
    {
      auto double mjd;
      auto Ulint pdate;
      register int cc_mode;
      register int i;
      register int n;
      auto int d;
      auto int m;
      auto int y = year;
      auto char *hdy_prefix;


      if (cc == (char *) NULL)
	cc_mode = 0;
      else if (!strcmp (cc, "AF"))
	cc_mode = 1;
      else if (!strcmp (cc, "AL"))
	cc_mode = 2;
      else if (!strcmp (cc, "AZ"))
	cc_mode = 3;
      else if (!strcmp (cc, "IR"))
	cc_mode = 4;
      else if (!strcmp (cc, "KG"))
	cc_mode = 5;
      else if (!strcmp (cc, "KZ"))
	cc_mode = 6;
      else if (!strcmp (cc, "TJ"))
	cc_mode = 7;
      else if (!strcmp (cc, "TM"))
	cc_mode = 8;
      else if (!strcmp (cc, "UZ"))
	cc_mode = 9;
      else if (!strcmp (cc, HD_MCNY))
	cc_mode = SPECIAL_VALUE;
      else
	abort ();		/* Error, invalid case! */
      if (!cc_mode || cc_mode == SPECIAL_VALUE)
	ptr_cc_id = _("Per");
      /*
         Detect the Persian_New_Year/Noruz, which is around the vernal equinox.
         The calculation of the vernal equinox is based on Iraq local time,
         not on UTC/GMT.  Tehran local time (not respecting Daylight Savings)
         is UTC/GMT-3.5.
       */
      mjd = equinox_solstice (NH_VE, &d, &m, &y, 3, 30);
      if (mjd != SPECIAL_VALUE)
	{
	  if (mjd - (Ulint) mjd >= 0.5)
	    /*
	       If the vernal equinox appears at or after noon,
	       the Persian_New_Year/Noruz is on the next day.
	     */
	    (void) next_date (&d, &m, &y);
	  mjd = day_of_year (d, m, y);
	  /*
	     Manage those holidays first, which cannot exceed the
	     year bounds of a Julian/Gregorian calendar year.
	   */
	  if (!cc_mode || cc_mode == SPECIAL_VALUE)
	    hdy_prefix = DIS_HLS_PREF;
	  else if (cc_mode == 2)
	    hdy_prefix = DIS_HLS_PREF2;
	  else
	    hdy_prefix = "+";
	  /*
	     Calculate the year number used in the Persian ``Jalaali'' calendar.
	   */
	  sprintf (s2, "%s %d", _(hd_text[HD_PERSIAN_NEW_YEARS_DAY].ht_text),
		   year - 621);
	  holiday (*init_data, detected, s2, ptr_cc_id, hdy_prefix, (int) mjd,
		   0, year, hd_elems, fday, count);
	  if (*init_data)
	    *init_data = FALSE;
	  if (!cc_mode)
	    {
	      for (i = 0; i < 5; i++)
		{
		  holiday (*init_data, detected,
			   _(hd_text[HD_MID_SPRING_FESTIVAL].ht_text),
			   ptr_cc_id, DIS_HLS_PREF, (int) mjd + 40 + i, 0,
			   year, hd_elems, fday, count);
		  holiday (*init_data, detected,
			   _(hd_text[HD_MIDSUMMER_DAY].ht_text), ptr_cc_id,
			   DIS_HLS_PREF, (int) mjd + 100 + i, 0, year,
			   hd_elems, fday, count);
		  holiday (*init_data, detected,
			   _(hd_text[HD_BRINGING_IN_THE_HARVEST_DAY].ht_text),
			   ptr_cc_id, DIS_HLS_PREF, (int) mjd + 175 + i, 0,
			   year, hd_elems, fday, count);
		  holiday (*init_data, detected,
			   _(hd_text[HD_BRINGING_HOME_THE_HERDS_DAY].ht_text),
			   ptr_cc_id, DIS_HLS_PREF, (int) mjd + 205 + i, 0,
			   year, hd_elems, fday, count);
		}
	      /*
	         Manage those holidays last, which can exceed
	         the year bounds of a Julian/Gregorian calendar year.
	       */
	      for (n = year - 1; n <= year; n++)
		if (n > 621)
		  {
		    y = n;
		    mjd = equinox_solstice (NH_VE, &d, &m, &y, 3, 30);
		    if (mjd != SPECIAL_VALUE)
		      {
			if (mjd - (Ulint) mjd >= 0.5)
			  (void) next_date (&d, &m, &y);
			pdate = date2num (d, m, y) - 1;
			/*
			   And manage all ``critical'' holidays.
			 */
			for (i = 0; i < 5; i++)
			  {
			    num2date (pdate + 286 + i, &d, &m, &y);
			    if (y == year)
			      holiday (*init_data, detected,
				       _(hd_text[HD_MID_YEAR_FESTIVAL].
					 ht_text), ptr_cc_id, DIS_HLS_PREF, d,
				       m, year, hd_elems, fday, count);
			    num2date (pdate + 361 + i, &d, &m, &y);
			    if (y == year)
			      holiday (*init_data, detected,
				       _(hd_text[HD_ALL_SOULS_DAY].ht_text),
				       ptr_cc_id, DIS_HLS_PREF, d, m, year,
				       hd_elems, fday, count);
			  }
			num2date (pdate + 310, &d, &m, &y);
			if (y == year)
			  holiday (*init_data, detected,
				   _(hd_text[HD_FESTIVAL_OF_SADEH].ht_text),
				   ptr_cc_id, DIS_HLS_PREF, d, m, year,
				   hd_elems, fday, count);
		      }
		  }
	    }
	}
    }
}



void
persian_mth (init_data, detected, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all Persian ``Jalaali'' calendar based months since AD 622.
     Note that this function calculates the solar ``Jalaali'' calendar
     astronomically and not arithmetically, so necessary calendar reformations
     which must apply in the furture for the arithmetical solution are avoided.
*/
{
  if (year > 621)
    {
      auto double mjd;
      auto Ulint pdate;
      register int i;
      register int n;
      auto int d;
      auto int m;
      auto int y;

      ptr_cc_id = _("Per");
      for (n = year - 1; n <= year; n++)
	if (n > 621)
	  {
	    y = n;
	    /*
	       Detect the Persian_New_Year/Noruz, which is around the vernal
	       equinox.  The calculation of the vernal equinox is based on
	       Iraq local time, not on UTC/GMT.  Tehran local time (not
	       respecting Daylight Savings) is UTC/GMT-3.5.
	     */
	    mjd = equinox_solstice (NH_VE, &d, &m, &y, 3, 30);
	    if (mjd != SPECIAL_VALUE)
	      {
		if (mjd - (Ulint) mjd >= 0.5)
		  /*
		     If the vernal equinox appears at or after noon,
		     the Persian_New_Year/Noruz is on the next day.
		   */
		  (void) next_date (&d, &m, &y);
		pdate = date2num (d, m, y);
		/*
		   And manage all Persian calendar months.
		 */
		for (i = MONTH_MIN; i <= MONTH_MAX; i++)
		  {
		    num2date (pdate, &d, &m, &y);
		    if (y == year)
		      ocs_monthtext (init_data, detected, ptr_cc_id, d, m,
				     year, FALSE, i, n - 621, SPECIAL_VALUE,
				     hd_elems, fday, count);
		    pdate += ((i < 7) ? 31 : 30);
		  }
	      }
	  }
    }
}



void
zodiacal_marker_hdy (init_data, detected, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all Zodiacal/Solar Seasonal Marker based holidays.
     Calculations are done for a line at a definite meridian expressed as a
     time value.  This time value is given by the global `time_hour_offset'
     variable and by the global `time_min_offset' variable.  If HOUR and MIN
     are set to zero, calculations are made for Universal Time (UTC/GMT).  If
     HOUR and MIN have a positive sign, UTC/GMT calculations are made for
     meridians East of Greenwich, otherwise for meridians West of Greenwich.
*/
{
  auto double n;
  auto double mjd;
  register int i;
  auto int the_min;
  auto int the_hour;
  auto int d;
  auto int m;
  auto int y = year;


  ptr_cc_id = _("Zod");
  /*
     All major zodiacal/solar term/Zhong-Qi seasonal markers.
   */
  for (i = 0, n = 0.0; i < MONTH_MAX; i++, n += 30.0)
    {
      mjd =
	sun_longitude (n, &d, &m, &y, time_hour_offset, time_min_offset,
		       FALSE);
      if (mjd != SPECIAL_VALUE)
	{
	  val2hours (mjd, &the_hour, &the_min, NULL);
	  sprintf (s2, "ZhongQi-%02d/%s[%03d] %02d%s%02d",
		   ((i + 1) % MONTH_MAX) + 1, zod_name[i], (int) n, the_hour,
		   time_sep, the_min);
	  holiday (*init_data, detected, s2, ptr_cc_id, DIS_HLS_PREF, d, m, y,
		   hd_elems, fday, count);
	  if (*init_data)
	    *init_data = FALSE;
	}
      else
	y = year;
    }
  /*
     All minor zodiacal/solar term/Jie-Qi seasonal markers.
   */
  for (i = 0, n = 15.0; i < MONTH_MAX; i++, n += 30.0)
    {
      mjd =
	sun_longitude (n, &d, &m, &y, time_hour_offset, time_min_offset,
		       FALSE);
      if (mjd != SPECIAL_VALUE)
	{
	  val2hours (mjd, &the_hour, &the_min, NULL);
	  sprintf (s2, "JieQi-%02d[%03d] %02d%s%02d",
		   ((i + 2) % MONTH_MAX) + 1, (int) n, the_hour, time_sep,
		   the_min);
	  holiday (*init_data, detected, s2, ptr_cc_id, DIS_HLS_PREF, d, m, y,
		   hd_elems, fday, count);
	}
      else
	y = year;
    }
}
