#ifndef __HD_DEFS_H
# define __HD_DEFS_H
/*!
*  \file hd-defs.h
*  \brief Holiday dates specific statements, macros, typedefs,
*  				global variable declarations...
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
*  $Id: hd-defs.h 3.01 2000/06/14 03:00:01 tom Exp $
*/



/*
*  Program specific preprocessor statements.
*/
/*! Maximum number of holidays per year (MONTH_MAX*MONTH_LAST). */
# define HD_MAX           372

/*! Maximum amount of eternal holidays in the table. */
# if HD_TOP20CC
#  define HD_ELEMS_MAX     1280
# else /* !HD_TOP20CC */
#  define HD_ELEMS_MAX     8192
# endif	/* !HD_TOP20CC */

/*! Maximum length of a holiday name inclusive `\0' ==> 31 characters effective textlength maximum. */
# define LEN_HD_NAME      40

/*! Identification for the Multicultural New Year's Day holidays. */
# define HD_MCNY          "MCNY"


/*
*  U.S. holidays observance MODE constants.
*/
/*!
*  Holiday is not observed:
*    Saturday -> Saturday
*    Sunday   -> Sunday
*/
# define SASA_SUSU    0
/*!
*  Holiday is observed:
*    Saturday -> Saturday
*    Sunday   -> next Monday
*/
# define SASA_SUMO    1
/*!
*  Holiday is observed:
*    Saturday -> next Monday
*    Sunday   -> next Monday
*/
# define SAMO_SUMO    2
/*!
*  Holiday is observed:
*    Saturday -> preceding Friday
*    Sunday   -> next Monday
*/
# define SAFR_SUMO    3
/*!
*  Holiday is observed:
*    Saturday -> preceding Friday (Veterans' Day only in US_CA)
*    Saturday -> Saturday (otherwise)
*    Sunday   -> next Monday
*/
# define SASAFR_SUMO  4



/*
*  ISO-3166 A2 (2-letters) country codes.
*/
# define CC_DE  "DE"
# define CC_GB  "GB"
# define CC_US  "US"



/*
*  The index of the supported country specific holiday texts.
*/
# define HD_1ST_ADVENT                         0
# define HD_1ST_SUNDAY_IN_LENT                 1
# define HD_2ND_ADVENT                         2
# define HD_2ND_SUNDAY_IN_LENT                 3
# define HD_3RD_ADVENT                         4
# define HD_3RD_SUNDAY_IN_LENT                 5
# define HD_4TH_ADVENT                         6
# define HD_4TH_SUNDAY_IN_LENT                 7
# define HD_7_5_3_FESTIVAL                     8
# define HD_ABORIGINES_DAY                     9
# define HD_ALL_FOOLS_DAY                      10
# define HD_ALL_SAINTS_DAY                     11
# define HD_ALL_SOULS_DAY                      12
# define HD_ALPHABET_DAY                       13
# define HD_ANNIVERSARY_OF_AUCKLAND            14
# define HD_ANNIVERSARY_OF_CANTERBURY_NORTH    15
# define HD_ANNIVERSARY_OF_CANTERBURY_SOUTH    16
# define HD_ANNIVERSARY_OF_CHATHAM_ISLANDS     17
# define HD_ANNIVERSARY_OF_HAWKES_BAY          18
# define HD_ANNIVERSARY_OF_MARLBOROUGH         19
# define HD_ANNIVERSARY_OF_NELSON              20
# define HD_ANNIVERSARY_OF_OTAGO               21
# define HD_ANNIVERSARY_OF_SOUTHLAND           22
# define HD_ANNIVERSARY_OF_TARANAKI            23
# define HD_ANNIVERSARY_OF_WELLINGTON          24
# define HD_ANNIVERSARY_OF_WESTLAND            25
# define HD_ANZAC_DAY                          26
# define HD_ARBOR_DAY                          27
# define HD_ARMED_FORCES_DAY                   28
# define HD_ARMISTICE_DAY                      29
# define HD_ASH_MONDAY                         30
# define HD_ASH_WEDNESDAY                      31
# define HD_BAHAI_ASCENSION_OF_ABDUL_BAHA      32
# define HD_BAHAI_ASCENSION_OF_BAHA_ULLAH      33
# define HD_BAHAI_BIRTH_OF_BAHA_ULLAH          34
# define HD_BAHAI_BIRTH_OF_THE_BAB             35
# define HD_BAHAI_DAY_OF_THE_COVENANT          36
# define HD_BAHAI_DECLARATION_OF_THE_BAB       37
# define HD_BAHAI_FIRST_DAY_OF_RIDVAN          38
# define HD_BAHAI_MARTYRDOM_OF_THE_BAB         39
# define HD_BAHAI_NEW_YEARS_DAY                40
# define HD_BAHAI_NINTH_DAY_OF_RIDVAN          41
# define HD_BAHAI_TWELFTH_DAY_OF_RIDVAN        42
# define HD_BANK_HOLIDAY                       43
# define HD_BASQUE_NATIONAL_DAY                44
# define HD_BATTLE_OF_BOYNE                    45
# define HD_BATTLE_OF_PUEBLA                   46
# define HD_BEGINNING_OF_FINANCIAL_YEAR        47
# define HD_BENITO_JUAREZ_DAY                  48
# define HD_BODHIDHARMAS_BIRTHDAY              49
# define HD_BOSSES_DAY                         50
# define HD_BOXING_DAY                         51
# define HD_BRINGING_HOME_THE_HERDS_DAY        52
# define HD_BRINGING_IN_THE_HARVEST_DAY        53
# define HD_BUDDHAS_BIRTHDAY                   54
# define HD_BURNS_NIGHT                        55
# define HD_CAMOES_DAY                         56
# define HD_CARNIVAL                           57
# define HD_CELTIC_BELTANE                     58
# define HD_CELTIC_IMBOLG                      59
# define HD_CELTIC_LUGHNASA                    60
# define HD_CELTIC_SAMHAIN                     61
# define HD_CHILDRENS_DAY                      62
# define HD_CHINESE_CYCLE                      63
# define HD_CHINESE_NEW_YEARS_DAY              64
# define HD_CHINESE_NEW_YEARS_EVE              65
# define HD_CHRISTMAS_DAY                      66
# define HD_CHRISTMAS_EVE                      67
# define HD_CHRISTS_ASCENSION_DAY              68
# define HD_CHRYSANTHENUM_DAY                  69
# define HD_CITIZENSHIP_DAY                    70
# define HD_CIVIC_HOLIDAY                      71
# define HD_COLUMBUS_DAY                       72
# define HD_COMING_OF_AGE_DAY                  73
# define HD_CONFUCIUS_BIRTHDAY                 74
# define HD_CONSTITUTION_ANNIVERSARY           75
# define HD_COPTIC_NEW_YEARS_DAY               76
# define HD_COSMONAUTS_DAY                     77
# define HD_CROSS_RAISING_DAY                  78
# define HD_CULTURE_DAY                        79
# define HD_DAY_AFTER_MID_AUTUMN_FESTIVAL      80
# define HD_DAY_AFTER_TOMB_SWEEPING_DAY        81
# define HD_DAY_OF_ANDALUCIA                   82
# define HD_DAY_OF_CATALONIA                   83
# define HD_DAY_OF_GALICIA                     84
# define HD_DAY_OF_PRAYER_AND_REPENTANCE       85
# define HD_DAY_OF_RECONCILIATION              86
# define HD_DEFENDERS_OF_MOTHERLAND_DAY        87
# define HD_DOUBLE_9_DAY                       88
# define HD_DRAGON_BOAT_FESTIVAL               89
# define HD_DYNASTY_DAY                        90
# define HD_EASTER_MONDAY                      91
# define HD_EASTER_SUNDAY                      92
# define HD_ECLIPSE_LUNAR                      93
# define HD_ECLIPSE_SOLAR                      94
# define HD_ECLIPSE_TYPE_ANNULAR               95
# define HD_ECLIPSE_TYPE_PARTIAL               96
# define HD_ECLIPSE_TYPE_PENUMBRAL             97
# define HD_ECLIPSE_TYPE_TOTAL                 98
# define HD_EINDHOVENS_LIBERATION              99
# define HD_ELECTION_DAY                       100
# define HD_EMPERORS_BIRTHDAY                  101
# define HD_EQUINOX_DAY                        102
# define HD_ETHIOPIC_NEW_YEARS_DAY             103
# define HD_FATHERS_DAY                        104
# define HD_FEAST_OF_CORPUS_CHRISTI            105
# define HD_FEAST_OF_FORTUNE                   106
# define HD_FEAST_OF_HEART_JESUS               107
# define HD_FESTIVAL_OF_LANTERNS               108
# define HD_FESTIVAL_OF_SADEH                  109
# define HD_FLAG_DAY                           110
# define HD_FLEMISH_CULTURE_DAY                111
# define HD_FOUNDING_OF_THE_COMMUNIST_PARTY    112
# define HD_FRENCH_CULTURE_DAY                 113
# define HD_FRENCH_REVOLUTIONARY               114
# define HD_FRIENDSHIP_DAY                     115
# define HD_GERMAN_UNITY_DAY                   116
# define HD_GOOD_FRIDAY                        117
# define HD_GOOD_SATURDAY                      118
# define HD_GRANDPARENTS_DAY                   119
# define HD_GREENERY_DAY                       120
# define HD_GROTTO_DAY                         121
# define HD_GROUNDHOG_DAY                      122
# define HD_GUADALUPE_VIRGINS_DAY              123
# define HD_GUY_FAWKES_DAY                     124
# define HD_HALLOWEEN                          125
# define HD_HEALTH_AND_SPORTS_DAY              126
# define HD_HEB_HANNUKAH                       127
# define HD_HEB_LAG_BOMER                      128
# define HD_HEB_PESACH                         129
# define HD_HEB_PURIM                          130
# define HD_HEB_ROSH_HASHANA                   131
# define HD_HEB_SHAVUOT                        132
# define HD_HEB_SHUSHAN_PURIM                  133
# define HD_HEB_SIMCHAT_TORAH                  134
# define HD_HEB_SUKKOT                         135
# define HD_HEB_TISHA_BAV                      136
# define HD_HEB_TU_BSHEVAT                     137
# define HD_HEB_YOM_HASHOAH                    138
# define HD_HEB_YOM_HAZIKARON                  139
# define HD_HEB_YOM_HA_AZMAUT                  140
# define HD_HEB_YOM_KIPPUR                     141
# define HD_HEB_YOM_YERUSHALAYIM               142
# define HD_HOLY_INNOCENTS_DAY                 143
# define HD_HOLY_TRINITY                       144
# define HD_HURAVEE_DAY                        145
# define HD_INDEPENDENCE_DAY                   146
# define HD_INDEPENDENCE_MOVEMENT_DAY          147
# define HD_INDEPENDENCE_PROCLAMATION          148
# define HD_INDIAN_NEW_YEARS_DAY               149
# define HD_ISL_AGA_KHANS_BIRTHDAY             150
# define HD_ISL_ASHURA                         151
# define HD_ISL_EID_AL_ADHA                    152
# define HD_ISL_EID_AL_FITR                    153
# define HD_ISL_EID_I_MILAD_UN_NABI            154
# define HD_ISL_GHADIR                         155
# define HD_ISL_IMAMAT_DAY                     156
# define HD_ISL_NEW_YEARS_DAY                  157
# define HD_ISL_NUZUL_AL_QURAN                 158
# define HD_ISL_QUDS_DAY                       159
# define HD_ISL_RAMADAN                        160
# define HD_ISL_SHAB_E_BARAT                   161
# define HD_ISL_SHAB_E_MIRAJ                   162
# define HD_ISL_SHAB_E_QADR                    163
# define HD_ISL_WAQF_AL_ARAFAT                 164
# define HD_JAPANESE_NEW_YEARS_DAY             165
# define HD_JAPANESE_NEW_YEARS_EVE             166
# define HD_JESUS_CIRCUMCISION                 167
# define HD_KWANZAA                            168
# define HD_LABOUR_DAY                         169
# define HD_LABOUR_THANKSGIVING_DAY            170
# define HD_LAO_TZES_BIRTHDAY                  171
# define HD_LIBERATION_DAY                     172
# define HD_MARINE_DAY                         173
# define HD_MARTINIMAS                         174
# define HD_MARTIN_LUTHER_KINGS_DAY            175
# define HD_MARTYRS_DAY                        176
# define HD_MARYS_ANNUNCIATION_DAY             177
# define HD_MARYS_ASCENSION_DAY                178
# define HD_MARYS_CANDLEMAS                    179
# define HD_MARYS_EXPECTATION                  180
# define HD_MARYS_IMMACULATE_CONCEPTION        181
# define HD_MARYS_MATERNITY                    182
# define HD_MARYS_NAME                         183
# define HD_MARYS_NATIVITY                     184
# define HD_MARYS_SACRIFICE                    185
# define HD_MARYS_VISITATION                   186
# define HD_MARY_BLESSED_VIRGIN                187
# define HD_MAUNDY_THURSDAY                    188
# define HD_MAY_DAY                            189
# define HD_MIDSUMMER_DAY                      190
# define HD_MID_AUTUMN_FESTIVAL                191
# define HD_MID_SPRING_FESTIVAL                192
# define HD_MID_YEAR_FESTIVAL                  193
# define HD_MOON_FIRST_QUARTER                 194
# define HD_MOON_FULL                          195
# define HD_MOON_LAST_QUARTER                  196
# define HD_MOON_NEW                           197
# define HD_MOTHERS_DAY                        198
# define HD_MOTHER_IN_LAWS_DAY                 199
# define HD_MUSIC_WATER_BANQUET                200
# define HD_NATIONAL_FOUNDATION_DAY            201
# define HD_NATIONAL_HOLIDAY                   202
# define HD_NATIONAL_MOURNING_DAY              203
# define HD_NEW_YEARS_DAY                      204
# define HD_NURSES_DAY                         205
# define HD_OLD_ARMENIC_NEW_YEARS_DAY          206
# define HD_OLD_EGYPTIC_NEW_YEARS_DAY          207
# define HD_ORTHODOX_NEW_YEARS_DAY             208
# define HD_OUR_LADY_APARECIDA_DAY             209
# define HD_PALM_SUNDAY                        210
# define HD_PARENTS_DAY                        211
# define HD_PASSION_SUNDAY                     212
# define HD_PEACE_FESTIVAL                     213
# define HD_PEACH_FESTIVAL                     214
# define HD_PENTECOST                          215
# define HD_PERSIAN_NEW_YEARS_DAY              216
# define HD_PRAYER_DAY                         217
# define HD_PRESIDENTIAL_INAUGURATION          218
# define HD_PRESIDENTS_DAY                     219
# define HD_PRESIDENTS_INFORM                  220
# define HD_PRESIDENT_LINCOLNS_BIRTHDAY        221
# define HD_PRINCESS_DAY                       222
# define HD_QUINQUAGESIMA_SUNDAY               223
# define HD_REFORMATION_DAY                    224
# define HD_REMEMBRANCE_DAY                    225
# define HD_RE_DAY                       226
# define HD_RESPECT_FOR_THE_AGED_DAY           227
# define HD_REVOLUTION_DAY                     228
# define HD_RIO_DE_JANEIRO_ANNIVERSARY         229
# define HD_ROGATION_SUNDAY                    230
# define HD_SAO_PAULO_ANNIVERSARY              231
# define HD_SAPPORO_SNOW_FESTIVAL              232
# define HD_SEASON_CHANGE                      233
# define HD_SEPTUAGESIMA_SUNDAY                234
# define HD_SEVEN_SLEEPERS_DAY                 235
# define HD_SEXAGESIMA_SUNDAY                  236
# define HD_SHROVE_TUESDAY                     237
# define HD_SOLSTICE_DAY                       238
# define HD_SOOT_SWEEPING_DAY                  239
# define HD_SPRING_FESTIVAL                    240
# define HD_START_OF_COMMON_MONTH              241
# define HD_START_OF_LEAP_MONTH                242
# define HD_STAR_FESTIVAL                      243
# define HD_ST_ANDREWS_DAY                     244
# define HD_ST_BARTHOLOMEW_DAY                 245
# define HD_ST_BERCHTOLDS_DAY                  246
# define HD_ST_DAVIDS_DAY                      247
# define HD_ST_EDWARDS_DAY                     248
# define HD_ST_GEORGES_DAY                     249
# define HD_ST_JAMES_DAY                       250
# define HD_ST_JOSEPHS_DAY                     251
# define HD_ST_LAURENTIUS_DAY                  252
# define HD_ST_NICHOLAS_DAY                    253
# define HD_ST_NICHOLAS_EVE                    254
# define HD_ST_PATRICKS_DAY                    255
# define HD_ST_PETER_AND_ST_PAUL               256
# define HD_ST_STEPHENS_DAY                    257
# define HD_ST_VALENTINES_DAY                  258
# define HD_SUNDAY_OF_THE_DEAD                 259
# define HD_SWALLOW_DAY                        260
# define HD_SWEETEST_DAY                       261
# define HD_SYLVESTER                          262
# define HD_TEACHERS_DAY                       263
# define HD_THAI_NEW_YEARS_DAY                 264
# define HD_THANKSGIVING_DAY                   265
# define HD_THE_KINGS_BIRTHDAY                 266
# define HD_THE_QUEENS_BIRTHDAY                267
# define HD_THREE_KINGS_DAY                    268
# define HD_TIRADENTES_DAY                     269
# define HD_TOMB_SWEEPING_DAY                  270
# define HD_TRANSFIGURATION_DAY                271
# define HD_TYNWALD_DAY                        272
# define HD_VALBORGS_EVE                       273
# define HD_VENICE_CARNIVAL                    274
# define HD_VETERANS_DAY                       275
# define HD_VICTORIA_DAY                       276
# define HD_VICTORY_DAY                        277
# define HD_WHIT_MONDAY                        278
# define HD_WOMENS_DAY                         279
# define HD_WOMENS_SHROVE_DAY                  280
# define HD_WORLD_ANIMAL_DAY                   281
# define HD_YOUTH_DAY                          282
# if !HD_TOP20CC
#  define HD_ADELAIDE_CUP_DAY                   283
#  define HD_ADMISSION_DAY                      284
#  define HD_ALASKA_DAY                         285
#  define HD_ALEKSIS_KIVI_DAY                   286
#  define HD_ALICE_SPRINGS_SHOW_DAY             287
#  define HD_ALL_SAINTS_EVE                     288
#  define HD_AMERICAS_DAY                       289
#  define HD_ANGAM_DAY                          290
#  define HD_ANNIVERSARY_OF_AMIRS_SUCCESSION    291
#  define HD_ANNIVERSARY_OF_ARMED_STRUGGLE      292
#  define HD_ANNIVERSARY_OF_GREEN_MARCH         293
#  define HD_ANTILLIAN_DAY                      294
#  define HD_ARAB_LEAGUE_DAY                    295
#  define HD_ARCHBISHOP_MAKARIOS_MEMORIAL_DAY   296
#  define HD_ARCHBISHOP_MAKARIOS_NAME_DAY       297
#  define HD_ARENGO_ANNIVERSARY                 298
#  define HD_ARMY_COUP_DAY                      299
#  define HD_ARRIVAL_OF_THE_SWISS               300
#  define HD_ARTIGAS_BIRTHDAY                   301
#  define HD_ASUNCION_DAY                       302
#  define HD_ATATURK_MEMORIAL_DAY               303
#  define HD_AUTHORITIYS_POWER_DAY              304
#  define HD_BAATH_REVOLUTION_DAY               305
#  define HD_BARON_BLISS_DAY                    306
#  define HD_BATAAN_DAY                         307
#  define HD_BATTLE_OF_ANGAMOS                  308
#  define HD_BATTLE_OF_BOYACA                   309
#  define HD_BATTLE_OF_CARABOBO                 310
#  define HD_BATTLE_OF_IQUIQUE                  311
#  define HD_BATTLE_OF_LAS_PIEDRAS              312
#  define HD_BATTLE_OF_NAEFELS                  313
#  define HD_BEGINNING_OF_SUMMER                314
#  define HD_BELGRADES_DAY                      315
#  define HD_BENNINGTON_BATTLE_DAY              316
#  define HD_BLESSING_OF_THE_WATER              317
#  define HD_BLOOMSDAY                          318
#  define HD_BOGANDA_DAY                        319
#  define HD_BONAIRE_DAY                        320
#  define HD_BOQUERON_BATTLE_DAY                321
#  define HD_BOTSWANA_DAY                       322
#  define HD_BOUNTY_DAY                         323
#  define HD_BUNKER_HILL_DAY                    324
#  define HD_BURNING_OF_JAN_HUS                 325
#  define HD_CANBERRA_DAY                       326
#  define HD_CARICOM_DAY                        327
#  define HD_CASIMIR_PULASKIS_BIRTHDAY          328
#  define HD_CASSINGA_DAY                       329
#  define HD_CAYENNE_FESTIVAL                   330
#  define HD_CESAR_CHAVEZ_DAY                   331
#  define HD_CHARTER_DAY                        332
#  define HD_CHIANG_KAI_SHEKS_BIRTHDAY          333
#  define HD_CHILDRENS_WHITE_SUNDAY             334
#  define HD_COLON_DAY                          335
#  define HD_COLORADO_DAY                       336
#  define HD_COMMONWEALTH_DAY                   337
#  define HD_COMPACT_DAY                        338
#  define HD_CONFEDERAL_AGREEMENT_DAY           339
#  define HD_CORONATION_DAY                     340
#  define HD_CUP_MATCH_DAY                      341
#  define HD_CUSTOM_CHIEFS_DAY                  342
#  define HD_C_MAPINDUZI_DAY                    343
#  define HD_DARWIN_SHOW_DAY                    344
#  define HD_DAY_OF_GOODWILL                    345
#  define HD_DAY_OF_STUDENTS_REVOLT             346
#  define HD_DAY_OF_THE_FREE_LAOS               347
#  define HD_DEATH_OF_H_CHRISTOPHE              348
#  define HD_DEATH_OF_J_DESSALINES              349
#  define HD_DEATH_OF_PRESIDENT_ABDALLAH        350
#  define HD_DEATH_OF_PRESIDENT_CHEIKH          351
#  define HD_DEATH_OF_PRESIDENT_SOILIH          352
#  define HD_DEATH_OF_QAID_I_AZAM               353
#  define HD_DEATH_OF_T_LOUVERTURE              354
#  define HD_DEFENDERS_DAY                      355
#  define HD_DIA_DEL_PADRE                      356
#  define HD_DIA_DE_LOS_MAESTROS                357
#  define HD_DISCOVERY_DAY                      358
#  define HD_DOWNFALL_OF_THE_DERGUE             359
#  define HD_DRY_SEASON_CELEBRATION             360
#  define HD_DR_SUN_YAT_SENS_BIRTHDAY           361
#  define HD_DUARTES_BIRTHDAY                   362
#  define HD_EASTER_TUESDAY                     363
#  define HD_ECONOMIC_LIBERATION_DAY            364
#  define HD_EIGHT_HOURS_DAY                    365
#  define HD_ELECTIONS_FOR_NATIONAL_ASSEMBLY    366
#  define HD_ERROL_BARROW_DAY                   367
#  define HD_EVACUATION_DAY                     368
#  define HD_EVE_OF_EPIPHANY                    369
#  define HD_EXPEDITION_OF_THE_33               370
#  define HD_E_HOSTOS_BIRTHDAY                  371
#  define HD_FAMILY_DAY                         372
#  define HD_FAO_DAY                            373
#  define HD_FARMERS_DAY                        374
#  define HD_FATHER_LEVAL_DAY                   375
#  define HD_FEAST_OF_OUR_THEOTOKOS             376
#  define HD_FISHERMANS_DAY                     377
#  define HD_FOUNDATION_OF_NPLA_DAY             378
#  define HD_FOUNDING_OF_PEOPLES_PARTY          379
#  define HD_GARIFUNA_DAY                       380
#  define HD_GENERAL_SAN_MARTINS_ANNIVERSARY    381
#  define HD_GOSPEL_DAY                         382
#  define HD_GUACANASTE_DAY                     383
#  define HD_GUSTAVUS_ADOLPHUS_DAY              384
#  define HD_HERITAGE_DAY                       385
#  define HD_HEROES_DAY                         386
#  define HD_HOBART_SHOW_DAY                    387
#  define HD_HOLY_WEDNESDAY                     388
#  define HD_HO_CHI_MINHS_BIRTHDAY              389
#  define HD_HUMAN_RIGHTS_DAY                   390
#  define HD_HURRICANE_SUPPLICATION_DAY         391
#  define HD_HURRICANE_THANKSGIVING_DAY         392
#  define HD_INDEPENDENCE_OF_CARTAGENA          393
#  define HD_INDEPENDENCE_OF_CUENCA             394
#  define HD_INDEPENDENCE_OF_GUAYAQUIL          395
#  define HD_INDEPENDENCE_OF_QUITO              396
#  define HD_INDIAN_ARRIVAL_DAY                 397
#  define HD_INTERNAL_AUTONOMY_DAY              398
#  define HD_JEFFERSON_DAVIS_BIRTHDAY           399
#  define HD_JUNE_HOLIDAY                       400
#  define HD_J_BARBOSAS_BIRTHDAY                401
#  define HD_J_CHILEMBWE_DAY                    402
#  define HD_J_DIEGOS_BIRTHDAY                  403
#  define HD_J_ROBERTS_BIRTHDAY                 404
#  define HD_KAMARAMPAKA_DAY                    405
#  define HD_KARTINI_DAY                        406
#  define HD_KATHERINE_SHOW_DAY                 407
#  define HD_KIEV_DAY                           408
#  define HD_KIM_IL_SUNGS_BIRTHDAY              409
#  define HD_KIM_JONG_ILS_BIRTHDAY              410
#  define HD_KING_KAMEHAMEHA_DAY                411
#  define HD_KING_RAMA_I_MEMORIAL_DAY           412
#  define HD_KING_RAMA_V_MEMORIAL_DAY           413
#  define HD_KNABENSCHIESSEN                    414
#  define HD_KONSTANTIN_AND_METHODIUS           415
#  define HD_LAPP_NATIONAL_HOLIDAY              416
#  define HD_LAUNCESTON_CUP_DAY                 417
#  define HD_LIBERTY_DAY                        418
#  define HD_LOSS_OF_MUSLIM_NATION              419
#  define HD_LUXEMBOURG_CITY_KERMIS             420
#  define HD_LYNDON_B_JOHNSON_DAY               421
#  define HD_L_RIVERAS_BIRTHDAY                 422
#  define HD_MADARAKA_DAY                       423
#  define HD_MAHATMA_GANDHIS_BIRTHDAY           424
#  define HD_MALVINAS_DAY                       425
#  define HD_MANILA_DAY                         426
#  define HD_MAPUTO_CITY_DAY                    427
#  define HD_MARIEN_NGOUABI_DAY                 428
#  define HD_MARYLAND_DAY                       429
#  define HD_MELBOURNE_CUP_DAY                  430
#  define HD_MERCHANT_DAY                       431
#  define HD_MIDSUMMERS_EVE                     432
#  define HD_MISSIONARY_DAY                     433
#  define HD_MI_CAREME_DAY                      434
#  define HD_MORAZAN_DAY                        435
#  define HD_MOSHESHOES_DAY                     436
#  define HD_MOTHERHOOD_AND_BEAUTY_DAY          437
#  define HD_NAMING_DAY                         438
#  define HD_NATIONAL_BUN_DAY                   439
#  define HD_NATIONAL_HEROES_DAY                440
#  define HD_NATIONAL_REDEMPTION_DAY            441
#  define HD_NATIONAL_RESISTANCE_DAY            442
#  define HD_NATIONAL_REVIVAL_DAY               443
#  define HD_NATIONAL_UNITY_DAY                 444
#  define HD_NDADAYE_DAY                        445
#  define HD_NEUTRALITY_DAY                     446
#  define HD_NEVADA_DAY                         447
#  define HD_NEW_REGIME_ANNIVERSARY             448
#  define HD_NOBEL_DAY                          449
#  define HD_NOI_DAY                            450
#  define HD_OAU_DAY                            451
#  define HD_OCTOBER_HOLIDAY                    452
#  define HD_OIL_INDUSTRY_DAY                   453
#  define HD_OUR_LADY_OF_ALTAGRACIA_DAY         454
#  define HD_OUR_LADY_OF_CAMARIN_DAY            455
#  define HD_OUR_LADY_OF_LAS_MERCEDES_DAY       456
#  define HD_OUR_LADY_OF_LOS_ANGELES_DAY        457
#  define HD_OUR_LADY_OF_SEVEN_SORROWS          458
#  define HD_OUR_LADY_OF_VICTORIES_DAY          459
#  define HD_PANAMA_CITY_DAY                    460
#  define HD_PANAMERICAN_DAY                    461
#  define HD_PARIS_PEACE_AGREEMENT_DAY          462
#  define HD_PATRIOTS_DAY                       463
#  define HD_PEOPLES_UPRISING_DAY               464
#  define HD_PEOPLE_POWER_DAY                   465
#  define HD_PHILIPPINE_USA_FRIENDSHIP          466
#  define HD_PICHINCHA_DAY                      467
#  define HD_PICNIC_DAY                         468
#  define HD_PIONEER_DAY                        469
#  define HD_POYA_DAY                           470
#  define HD_PRINCE_KALANIANAOLE_DAY            471
#  define HD_PRINCE_OF_WALES_BIRTHDAY           472
#  define HD_PROCLAMATION_DAY                   473
#  define HD_QAID_I_AZAMS_BIRTHDAY              474
#  define HD_RATU_SUKUMA_DAY                    475
#  define HD_RECREATION_DAY                     476
#  define HD_REFERENDUM_DAY                     477
#  define HD_REGATTA_DAY                        478
#  define HD_REGENCY_EXCHANGE                   479
#  define HD_RESTORATION_DAY                    480
#  define HD_RETURN_DAY                         481
#  define HD_REUNIFICATION_DAY                  482
#  define HD_RIZAL_DAY                          483
#  define HD_RWAGASORE_DAY                      484
#  define HD_SABA_DAY                           485
#  define HD_SAN_JACINTO_DAY                    486
#  define HD_SAN_JOSE_DAY                       487
#  define HD_SAR_ESTABLISHMENT_DAY              488
#  define HD_SCHOOL_HOLIDAY                     489
#  define HD_SECHSELAEUTEN                      490
#  define HD_SEPARATION_DAY                     491
#  define HD_SERETSE_KHAMA_DAY                  492
#  define HD_SETTLERS_DAY                       493
#  define HD_SEWARDS_DAY                        494
#  define HD_SHAHEED_DAY                        495
#  define HD_SHEEP_FESTIVAL                     496
#  define HD_SHEIKH_ZAYEDS_ASCENSION_DAY        497
#  define HD_SIMON_BOLIVAR_DAY                  498
#  define HD_SINAI_LIBERATION_DAY               499
#  define HD_SLAVE_LIBERATION_DAY               500
#  define HD_SNIFF_THE_BREEZE_DAY               501
#  define HD_SOLIDARITY_DAY                     502
#  define HD_SOMERS_DAY                         503
#  define HD_STATIA_AMERICA_DAY                 504
#  define HD_STUDENTS_DAY                       505
#  define HD_ST_AGATA_DAY                       506
#  define HD_ST_CANUTES_DAY                     507
#  define HD_ST_CEDILIAS_DAY                    508
#  define HD_ST_CHARLES_DAY                     509
#  define HD_ST_DEMETRIUS_DAY                   510
#  define HD_ST_DEVOTE_DAY                      511
#  define HD_ST_ELIAHS_DAY                      512
#  define HD_ST_JEAN_BAPTISTE_DAY               513
#  define HD_ST_LUCIAS_DAY                      514
#  define HD_ST_MARGUERITES_DAY                 515
#  define HD_ST_MAROONS_DAY                     516
#  define HD_ST_MICHAELS_DAY                    517
#  define HD_ST_OLAVS_DAY                       518
#  define HD_ST_OLAVS_EVE                       519
#  define HD_ST_PAULS_SHIPSWRECK                520
#  define HD_ST_ROSE_OF_LIMA                    521
#  define HD_ST_URSULAS_DAY                     522
#  define HD_ST_VINCENT_DE_PAULS_DAY            523
#  define HD_SUEZ_VICTORY_DAY                   524
#  define HD_SULTAN_QABOOS_BIRTHDAY             525
#  define HD_SVETITSKHOVLOBA                    526
#  define HD_SWEDEN_DAY                         527
#  define HD_S_DOES_BIRTHDAY                    528
#  define HD_TENNANT_CREEK_SHOW_DAY             529
#  define HD_TERRITORY_DAY                      530
#  define HD_THE_CROWN_PRINCESSES_BIRTHDAY      531
#  define HD_THE_CROWN_PRINCESSES_NAME_DAY      532
#  define HD_THE_CROWN_PRINCES_BIRTHDAY         533
#  define HD_THE_KINGS_NAME_DAY                 534
#  define HD_THE_QUEENS_NAME_DAY                535
#  define HD_TOPOU_I_DAY                        536
#  define HD_TOWN_MEETING_DAY                   537
#  define HD_TRADITIONAL_DAY                    538
#  define HD_TRANSFER_DAY                       539
#  define HD_TRUMAN_DAY                         540
#  define HD_UNION_DAY                          541
#  define HD_UNION_WITH_SWEDEN_DISSOLVED        542
#  define HD_UNITED_NATIONS_DAY                 543
#  define HD_UNITY_FACTORY_DAY                  544
#  define HD_VICTOR_SCHOELCHER_DAY              545
#  define HD_VIDOVDAN                           546
#  define HD_WEST_VIRGINIA_DAY                  547
#  define HD_WHITSUN_EVE                        548
#  define HD_W_TUBMANS_BIRTHDAY                 549
#  define HD_YAP_DAY                            550
#  define HD_DAY_OF_CZECH_STATEHOOD             551
# endif	/* !HD_TOP20CC */



/*
*  Type definitions.
*/
/*
*  The country code record.
*/
typedef struct country_code_type
{
  /*
     The ISO-3166 2-characters country code identifer,
     plus an optionally trailing territory text `_TT'.
   */
  char *cc_id;

  /* Description text of the country code; the country name. */
  char *cc_info;

  /* The country code specific holiday handling function. */
  void (*Cc_hdy_handler) ();
}
Cc_struct;

/*
*  The country specific holiday text record.
*/
typedef struct holiday_text_type
{
  /* The index of the country specific holiday text. */
  int ht_id;

  /* The country specific holiday text itself. */
  char *ht_text;
}
Ht_struct;

/*
*  The country code specific holiday handling function type.
*/
typedef
  void
  (*Cc_hdy_handler)
__P_ ((Bool *, const Bool, int, const int, int *, const int, const int));



/*
*  GLOBAL variables declarations.
*/
/*! The table of country specific holidays. */
extern const Cc_struct cc_holidays[];

/*! The table which contains all the holiday texts. */
extern const Ht_struct hd_text[];

/*! U.S. holidays observance mode. */
extern int observe_us_hdy;

/*! The ISO-3166 character code used in the country specific `hdy_*()' functions. */
extern char *ptr_cc_id;

/*! Toggle which indicates to use a different CC in output. */
extern Bool use_other_cc;
#endif /* __HD_DEFS_H */
