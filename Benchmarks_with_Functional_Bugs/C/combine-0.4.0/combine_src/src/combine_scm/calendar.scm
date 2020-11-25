;;; calendar.scm --- Calendar functions.
;; Copyright (C) 2002, 2013 Daniel P. Valentine
;; Portions Copyright (C) 1988, 1989, 1990, 1991, 1992, 1993, 1994, 1995, 1997
;;        Free Software Foundation, Inc.

;; Author: Daniel P. Valentine <dpv@world.std.com>
  
;; calendar.scm is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 3, or (at your option)
;; any later version.
  
;; calendar.scm is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with calendar.scm; see the file COPYING.  If not, write to the
;; Free Software Foundation, Inc., 59 Temple Place - Suite 330,
;; Boston, MA 02111-1307, USA. 

;; Much of this file is adapted from the file calendar.el and the
;; supporting programs, which are a part of GNU Emacs.  Its original
;; author is Edward M. Reingold <reingold@cs.uiuc.edu>.  Contributing
;; authors of some parts are Stewart M. Clamen <clamen@cs.cmu.edu> for
;; the Mayan calendar

;; In general, Reingold uses as a reference his book ``Calendrical
;; Calculations'' by Nachum Dershowitz and Edward M. Reingold,
;; Cambridge University Press (1997).  Some also refer to his article
;; ``Calendrical Calculations, Part II: Three Historical Calendars''
;; by E. M. Reingold, N. Dershowitz, and S. M. Clamen,
;; Software--Practice and Experience, Volume 23, Number 4 (April,
;; 1993), pages 383-404.  I have tried to include his other references
;; in the local comments for each section.

(define-module (combine_scm calendar))

;;; Customizable Variables

;; These variables can be adjusted to show locality or to add language
;; support.

;; Names of days in the gregorian 7-day week.  The alist is preferred
;; for language support.
(define gregorian-day-name-alist '( (en #("Sunday" "Monday" "Tuesday" "Wednesday" "Thursday" "Friday" "Saturday"))
				    (de #("Sonntag" "Montag" "Dienstag" "Mitwoch" "Donnerstag" "Freitag" "Samstag"))
				    (fr #("Dimanche" "Lundi" "Mardi" "Mercredi" "Jeudi" "Vendredi" "Je l'ai oublie'"))))
(define calendar-day-name-array '#("Sunday" "Monday" "Tuesday" "Wednesday" "Thursday" "Friday" "Saturday"))
(define calendar-day-abbrev-array '#("Sun" "Mon" "Tue" "Wed" "Thu" "Fri" "Sat"))

;; Names of months in the gregorian calendar.  The alist is preferred
;; for language support.
(define gregorian-month-name-alist '( (en #("January" "February" "March" "April" "May" "June" "July" "August" "September" "October" "November" "December"))
				      (de #("Januar" "Februar" "Ma:rz" "April" "Mai" "Juni" "Juli" "August" "September" "Oktober" "November" "Dezember"))
				      (fr #("Janvier" "Fevrier" "Mars" "Avril" "Mai" "Juin" "Juillet" "Auguste" "Septembre" "Octobre" "Novembre" "Decembre"))))
(define calendar-month-name-array '#("January" "February" "March" "April" "May" "June" "July" "August" "September" "October" "November" "December"))
(define calendar-month-abbrev-array '#("Jan" "Feb" "Mar" "Apr" "May" "Jun" "Jul" "Aug" "Sep" "Oct" "Nov" "Dec"))


;; Names of Islamic calendar months
(define calendar-islamic-month-name-array 
  '#("Muharram" "Safar" "Rabi I" "Rabi II" "Jumada I" "Jumada II" "Rajab" "Sha'ban" "Ramadan"
     "Shawwal" "Dhu al-Qada" "Dhu al-Hijjah"))

;; Names of Hebrew calendar months
(define calendar-hebrew-month-name-array-common-year
  '#("Nisan" "Iyar" "Sivan" "Tammuz" "Av" "Elul" "Tishri"
   "Heshvan" "Kislev" "Teveth" "Shevat" "Adar"))

(define calendar-hebrew-month-name-array-leap-year
  '#("Nisan" "Iyar" "Sivan" "Tammuz" "Av" "Elul" "Tishri"
   "Heshvan" "Kislev" "Teveth" "Shevat" "Adar I" "Adar II"))

;; Names of stems for Chinese calendar
(define chinese-calendar-celestial-stem
  '#("Jia" "Yi" "Bing" "Ding" "Wu" "Ji" "Geng" "Xin" "Ren" "Gui"))

(define chinese-calendar-terrestrial-branch
  '#("Zi" "Chou" "Yin" "Mao" "Chen" "Si" "Wu" "Wei" "Shen" "You" "Xu" "Hai"))

;; Note: There are some customizable Chinese calendar options below in
;; the Chinese calendar section: chinese-calendar-time-zone,
;; chinese-calendar-location-name,
;; chinese-calendar-daylight-time-offset,
;; chinese-calendar-standard-time-zone-name,
;; chinese-calendar-daylight-time-zone-name,
;; chinese-calendar-daylight-savings-starts,
;; chinese-calendar-daylight-savings-ends,
;; chinese-calendar-daylight-savings-starts-time,
;; chinese-calendar-daylight-savings-ends-time

;; Names of lunar phases
(define lunar-phase-name-alist
  '( (en #("New Moon" "First Quarter Moon" "Full Moon" "Last Quarter Moon"))))

;; Names of solar season change events

;; solar-n-hemi-seasons-alist --- List of season changes for the northern
;; hemisphere.
(define solar-n-hemi-seasons-alist
  '( (en #("Vernal Equinox" "Summer Solstice" "Autumnal Equinox" "Winter Solstice"))))

;; solar-s-hemi-seasons-alist --- List of season changes for the southern
;; hemisphere.
(define solar-s-hemi-seasons-alist
  '( (en #("Autumnal Equinox" "Winter Solstice" "Vernal Equinox" "Summer Solstice"))))

;; Names from the French Revolutionary calendar

;; So far, I assume that there are not names for these in other
;; languages, although some could certainly be translated.

(define french-calendar-month-name-array
  '#("Vende'miaire" "Brumaire" "Frimaire" "Nivo^se" "Pluvio^se" "Vento^se"
   "Germinal" "Flore'al" "Prairial" "Messidor" "Thermidor" "Fructidor"))

(define french-calendar-multibyte-month-name-array
  '#("Vendémiaire" "Brumaire" "Frimaire" "Nivôse" "Pluviôse" "Ventôse"
   "Germinal" "Floréal" "Prairial" "Messidor" "Thermidor" "Fructidor"))

(define french-calendar-day-name-array
  '#("Primidi" "Duodi" "Tridi" "Quartidi" "Quintidi" "Sextidi" "Septidi"
   "Octidi" "Nonidi" "Decadi"))

(define french-calendar-multibyte-special-days-array
  '#("de la Vertu" "du Génie" "du Labour" "de la Raison" "de la Récompense"
   "de la Révolution"))

(define french-calendar-special-days-array
  '#("de la Vertu" "du Ge'nie" "du Labour" "de la Raison" "de la Re'compense"
   "de la Re'volution"))

;; Names for Coptic and Ethiopic months

(define coptic-calendar-month-name-array
  '#("Tut" "Babah" "Hatur" "Kiyahk" "Tubah" "Amshir" "Baramhat" "Barmundah"
   "Bashans" "Baunah" "Abib" "Misra" "al-Nasi"))

(define ethiopic-calendar-month-name-array
  '#("Maskaram" "Teqemt" "Khedar" "Takhsas" "Ter" "Yakatit" "Magabit" "Miyazya"
   "Genbot" "Sane" "Hamle" "Nahas" "Paguem"))

;; Names for Persian months

(define persian-calendar-month-name-array
  '#("Farvardin" "Ordibehest" "Xordad" "Tir" "Mordad" "Sahrivar" "Mehr" "Aban"
   "Azar" "Dey" "Bahman" "Esfand"))

;; Names for Mayan haabs and tzolkins

(define calendar-mayan-haab-month-name-array
  '#("Pop" "Uo" "Zip" "Zotz" "Tzec" "Xul" "Yaxkin" "Mol" "Chen" "Yax"
   "Zac" "Ceh" "Mac" "Kankin" "Muan" "Pax" "Kayab" "Cumku"))

(define calendar-mayan-tzolkin-names-array
  '#("Imix" "Ik" "Akbal" "Kan" "Chicchan" "Cimi" "Manik" "Lamat" "Muluc" "Oc"
   "Chuen" "Eb" "Ben" "Ix" "Men" "Cib" "Caban" "Etznab" "Cauac" "Ahau"))




;; Customizable constants for solar calculations.
(define calendar-longitude 8.6)  ;; Local longitude
(define calendar-latitude 50.05) ;; Local latitude
(define calendar-time-zone 60)   ;; Minutes difference from UTC (not clear if it is positive or negative in each direction)

;; solar-error --- *Tolerance (in minutes) for sunrise/sunset
;; calculations.
;;
;; A larger value makes the calculations for sunrise/sunset faster,
;; but less accurate.  The default is half a minute (30 seconds), so
;; that sunrise/sunset times will be correct to the minute.
;;
;; It is useless to set the value smaller than 4*delta, where delta is
;; the accuracy in the longitude of the sun (given by the function
;; `solar-ecliptic-coordinates') in degrees since (delta/360) x
;; (86400/60) = 4 x delta.  At present, delta = 0.01 degrees, so the
;; value of the variable `solar-error' should be at least 0.04 minutes
;; (about 2.5 seconds).
(define solar-error 0.5)

;; Absolute date of starting date of system clock.  For use in
;; interpreting second counts returned by the system.
(define calendar-system-time-basis-date '(1 1 1970))



;;; General utility functions used by multiple calendars.

;; Parsing the date list format.  The format is a list with elements
;; MONTH, DAY, and YEAR.
(define (extract-calendar-month date)
  "The month portion of list DATE in format (MONTH DAY YEAR)"
  (car date))
    
(define (extract-calendar-day date)
  "The day portion of list DATE in format (MONTH DAY YEAR)"
  (car (cdr date)))

(define (extract-calendar-year date)
  "The year portion of list DATE in format (MONTH DAY YEAR)"
  (car (cdr (cdr date))))

(define (calendar-mod m n)
  "Non-negative remainder of M/N with N instead of 0."
  (1+ (mod (1- m) n)))

;; Get the time zone and offset like the function of the same name in
;; Emacs.  Here I use the date command in the system to get me the
;; information I need.
(define (current-time-zone . time)
  (let* ((now (if (null? time) 
		  (current-time)
		  (car time)))
	 (btime (localtime now)))
    (list (* -1 (quotient (tm:gmtoff btime) 60)) ;; Convert seconds to minutes. (And invert sign)
	  (tm:zone btime))))

;; nil --- An empty list.
(define nil '())

;; mod --- A modulus function that works with real numbers.
(define (mod x y)
  (cond ((= y 0) #f)
	((and (integer? x) (integer? y) (exact? x) (exact? y)) (modulo x y))
	(#t (- x (* y (floor (/ x y)))))))


;;; Degree Conversion

;; degrees-to-radians --- conversion required by some of the calendar
;; functions.  I was too lazy to see if there was a function in the
;; code provided in Emacs, so I wrote my own. (Degrees devided by 180
;; time pi)
(define (degrees-to-radians deg)
  (/ (* deg (atan 1))
     45.0))

;; Conversion from radiand to degrees required for some of the
;; calendar functions.  Laziness here, too. (Radians divided by pi
;; times 180)
(define (radians-to-degrees rad)
  (* (/ rad (atan 1)) 45.0))




;;; Gregorian Calendar

;; Any multiple of 4 except multiples of 100 except multiples of 400.
;; (1996 and 2000 but not 1900)
(define (calendar-leap-year? year)
  "Returns #t if YEAR is a gregorian leap year."
  (and (zero? (modulo year 4))
       (or (not (zero? (modulo year 100)))
           (zero? (modulo year 400)))))

(define (calendar-last-day-of-month month year)
  "Returns the last day of the given MONTH in the given YEAR"
  (if (and (= month 2) (calendar-leap-year? year))
      29
    (vector-ref '#(31 28 31 30 31 30 31 31 30 31 30 31) (1- month))))

;; calendar-day-number -- Returns the number of the day within the
;; year.  Requires the date to be list of (month day year).  For
;; example, (calendar-day-number '(1 1 1987)) returns the value 1,
;; while (calendar-day-number '(12 31 1980)) returns 366.  The
;; calculation assumes 31 days per preceding month and then adjusts
;; accordingly for the number of non-31-day months that have passed.
(define (calendar-day-number date)
  "Returns the number of the day within the year from (MONTH DAY YEAR) list"
  (let* ((month (extract-calendar-month date))
	 (day (extract-calendar-day date))
	 (year (extract-calendar-year date))
         (day-of-year (+ day (* 31 (1- month)))))
    (if (> month 2)
	(begin
	  (set! day-of-year (- day-of-year (quotient (+ 23 (* 4 month)) 10)))
	  (if (calendar-leap-year? year)
	      (set! day-of-year (1+ day-of-year)))))
    day-of-year))

;; calendar-absolute-from-gregorian -- converts a standard
;; representation of a gregorian date to a count of days since
;; (Sunday) 31 Dec 1 BC.  This assumes that the gregorian leap years
;; held into the past.
(define (calendar-absolute-from-gregorian date)
  "Converts Gregorian DATE (M D Y) to day count"
  (let ((prior-years (1- (extract-calendar-year date))))
    (+ (calendar-day-number date);; Days this year
       (* 365 prior-years);;        + Days in prior years
       (quotient prior-years 4);;          + Julian leap years
       (- (quotient prior-years 100));;    - century years
       (quotient prior-years 400))));;     + Gregorian leap years

;; calendar-gregorian-from-absolute --- Calculate the gregorian date
;; in a list (month day year) from a day count since 31 Dec 1 BC.
(define (calendar-gregorian-from-absolute datenum)
  "Converts day count DATENUM to Gregorian (M D Y)" 
  (let* ((d0 (1- datenum));;            The number of the day before.
         (n400 (quotient d0 146097));;  The number of preceding 400-year periods.
         (d1 (modulo d0 146097));;      Everything in the current 400-year period.
         (n100 (quotient d1 36524));;   The number of preceding 100-year periods (since the last 400).
         (d2 (modulo d1 36524));;       Everything in the current 100-year period.
         (n4 (quotient d2 1461));;      The number of preceding 4-year periods this century.
         (d3 (modulo d2 1461));;        Everything in the current 4-year period.
         (n1 (quotient d3 365));;       The number of years since the last leap year.
         (day (1+ (modulo d3 365)));;   The number of days this year.
         (year (+ (* 400 n400) (* 100 n100) (* n4 4) n1)));; The number of years before this year.
    (if (or (= n100 4) (= n1 4));;      If we calculated 4 centuries or 4 years since the last 400-year 
;;                                      or leap-year period, then 365 of this year's 366 days have passed, 
;;                                      and it must be December 31.
        (list 12 31 year)
	(let ((year (1+ year));;          Otherwise the year is one more than the count of past years.
	      (month 1));;                Loop through the months until we find one that the day count fits into.
	  (while (let ((mdays (calendar-last-day-of-month month year)))
		   (and (< mdays day)
			(set! day (- day mdays))))
		 (set! month (1+ month)))
	  (list month day year)))))


;; The absolute date of Nth DAYNAME in MONTH, YEAR before/after
;; optional DAY.  A DAYNAME of 0 means Sunday, 1 means Monday, and so
;; on.  If N<0, return the Nth DAYNAME before MONTH DAY, YEAR
;; (inclusive).  If N>0, return the Nth DAYNAME after MONTH DAY, YEAR
;; (inclusive).
;;
;; If DAY is omitted, it defaults to 1 if N>0, and MONTH's last day
;; otherwise.
(define (calendar-nth-named-absday n dayname month year . day)
  "The day count of the Nth DAYNAME in MONTH, YEAR counting from optional DAY"
  (if (> n 0)
      (+ (* 7 (1- n))
	 (calendar-dayname-on-or-before
	  dayname
	  (+ 6 (calendar-absolute-from-gregorian
		(list month (if (null? day) 1 (car day)) year)))))
      (+ (* 7 (1+ n))
	 (calendar-dayname-on-or-before
	  dayname
	  (calendar-absolute-from-gregorian
	   (list month
		 (if (null? day) (calendar-last-day-of-month month year) (car day))
		 year))))))

;; The date of Nth DAYNAME in MONTH, YEAR before/after optional DAY.
;; A DAYNAME of 0 means Sunday, 1 means Monday, and so on.  If N<0,
;; return the Nth DAYNAME before MONTH DAY, YEAR (inclusive).  If N>0,
;; return the Nth DAYNAME after MONTH DAY, YEAR (inclusive).
;;
;; If DAY is omitted, it defaults to 1 if N>0, and MONTH's last day
;; otherwise.
(define (calendar-nth-named-day n dayname month year . day)
  (calendar-gregorian-from-absolute
   (if (null? day)
       (calendar-nth-named-absday n dayname month year)
       (calendar-nth-named-absday n dayname month year day))))


;; Returns the absolute date of the DAYNAME on or before absolute
;; DATE.  DAYNAME=0 means Sunday, DAYNAME=1 means Monday, and so on.
;;
;; Note: Applying this function to d+6 gives us the DAYNAME on or
;; after an absolute day d.  Similarly, applying it to d+3 gives the
;; DAYNAME nearest to absolute date d, applying it to d-1 gives the
;; DAYNAME previous to absolute date d, and applying it to d+7 gives
;; the DAYNAME following absolute date d.
(define (calendar-dayname-on-or-before dayname date)
  "Returns the day count of the day of the week DAYNAME on or before DATE"
  (- date (modulo (- date dayname) 7)))

;; Returns t if the DATE1 and DATE2 are the same.
(define (calendar-date-equal date1 date2)
  "Returns #t is DATE1 and DATE2 are the same date list (M D Y)"
  (and
   (= (extract-calendar-month date1) (extract-calendar-month date2))
   (= (extract-calendar-day date1) (extract-calendar-day date2))
   (= (extract-calendar-year date1) (extract-calendar-year date2))))

(define calendar-system-time-basis
  (calendar-absolute-from-gregorian calendar-system-time-basis-date))


;;; Julian Dates

;; calendar-julian-from-absolute --- Convert a day count to a Julian
;; date.  This is adapted from the calendar-gregorian-from-absolute
;; rather than the function of the same name in the Emacs calendar
;; because it should be faster and I unrderstand it better.
(define (calendar-julian-from-absolute datenum)
  "The Julian (M D Y) list corresponding to day count DATENUM"
  (let* ((d0 (1+ datenum));;            The number of the day after (there is a 2-day offset in the Julian dates that I don't yet understand).
         (n4 (quotient d0 1461));;      The number of preceding 4-year periods this century.
         (d3 (modulo d0 1461));;        Everything in the current 4-year period.
         (n1 (quotient d3 365));;       The number of years since the last leap year.
         (day (1+ (modulo d3 365)));;   The number of days this year.
         (year (+ (* n4 4) n1)));; The number of years before this year.
    (if (= n1 4);;                      If we calculated 4 years since the last leap-year period, then
;;                                      365 of this year's 366 days have passed, and it must be December 31.
        (list 12 31 year)
	(let ((year (1+ year));;          Otherwise the year is one more than the count of past years.
	      (month 1));;                Loop through the months until we find one that the day count fits into.
	  (while (let ((mdays (calendar-last-day-of-month month year)))
		   (and (< mdays day)
			(set! day (- day mdays))))
		 (set! month (1+ month)))
	  (list month day year)))))

;; calendar-absolute-from-julian --- Take a julian date in (month day
;; year) format and turn it into a day count
(define (calendar-absolute-from-julian date)
  "The day count corresponding to the Julian DATE (M D Y) list"
  (let ((month (extract-calendar-month date))
        (day (extract-calendar-day date))
        (year (extract-calendar-year date)))
    (+ (calendar-day-number date)
       (if (and (= (modulo year 100) 0)
                (not (= (modulo year 400) 0))
                (> month 2))
           1 0);; Correct for Julian but not Gregorian leap year.
       (* 365 (1- year))
       (quotient (1- year) 4)
       -2)))


;;; Islamic Dates

;; The start date of the Islamic calendar = July 16, 622
;; A.D. (Julian).  [Note: Reingold had August 29, 284 A.D., but that
;; appears to have been mistakenly copied from the Coptic comments.  I
;; changed the comment to match the calculation below.]
(define calendar-islamic-epoch (calendar-absolute-from-julian '(7 16 622)))

;; islamic-calendar-leap-year? --- Is this year a leap year?
(define (islamic-calendar-leap-year? year)
  (memq (modulo year 30)
        (list 2 5 7 10 13 16 18 21 24 26 29)))

;; islamic-calendar-last-day-of-month --- For a given month and year,
;; returns the number of days in the month.
(define (islamic-calendar-last-day-of-month month year)
  "The last day of MONTH in YEAR (in the Islamic calendar)"
  (cond
   ((memq month (list 1 3 5 7 9 11)) 30)
   ((memq month (list 2 4 6 8 10)) 29)
   (#t (if (islamic-calendar-leap-year? year) 30 29))))

;; islamic-calendar-day-number --- Returns the day within the year of
;; the Islamic date DATE
(define (islamic-calendar-day-number date)
  "Return the day number within the year of the Islamic date DATE."
  (let* ((month (extract-calendar-month date))
	 (day (extract-calendar-day date)))
    (+ (* 30 (quotient month 2))
       (* 29 (quotient (1- month) 2))
       day)))

;; calendar-absolute-from-islamic --- Calculate a day count from a
;; given islamic date.
(define (calendar-absolute-from-islamic date)
  "Day count corresponding Islamic DATE"
  (let* ((month (extract-calendar-month date))
         (day (extract-calendar-day date))
         (year (extract-calendar-year date))
         (y (modulo year 30))
         (leap-years-in-cycle
          (cond
           ((< y 3) 0)  ((< y 6) 1)  ((< y 8) 2)  ((< y 11) 3) ((< y 14) 4)
           ((< y 17) 5) ((< y 19) 6) ((< y 22) 7) ((< y 25) 8) ((< y 27) 9)
           (#t 10))))
    (+ (islamic-calendar-day-number date);; days so far this year
       (* (1- year) 354)                 ;; days in all non-leap years
       (* 11 (quotient year 30))         ;; leap days in complete cycles
       leap-years-in-cycle               ;; leap days this cycle
       (1- calendar-islamic-epoch))))    ;; days before start of calendar


;; calendar-islamic-from-absolute --- Calculate a (month day year)
;; list in Islamic numbering from a day count.
(define (calendar-islamic-from-absolute date)
  "The Islamic (M D Y) date corresponding to day count DATE"
  (if (< date calendar-islamic-epoch)
      (list 0 0 0);; pre-Islamic date
      (let* ((approx (quotient (- date calendar-islamic-epoch)
			355));; Approximation from below.
	     (year           ;; Search forward from the approximation.
	      (+ approx
		 (let ((y approx)
		       (sum 0))
		   (while (>= date (calendar-absolute-from-islamic (list 1 1 (1+ y))))
			  (set! sum (+ sum 1))
			  (set! y (1+ y)))
		   sum)))
	     (month          ;; Search forward from Muharram.
	      (1+ (let ((m 1)
			(sum 0))
		    (while (> date (calendar-absolute-from-islamic (list m (islamic-calendar-last-day-of-month m year) year)))
			   (set! sum (+ sum 1))
			   (set! m (1+ m)))
		    sum)))
	     (day            ;; Calculate the day by subtraction.
	      (- date
		 (1- (calendar-absolute-from-islamic (list month 1 year))))))
	(list month day year))))


;;; Hebrew Calendar

(define (calendar-hebrew-from-absolute date)
  "The Hebrew date (month day year) corresponding to absolute DATE."
  (let* ((greg-date (calendar-gregorian-from-absolute date))
         (month (vector-ref '#(9 10 11 12 1 2 3 4 7 7 7 8)
			    (1- (extract-calendar-month greg-date))))
         (day #f)
         (year (+ 3760 (extract-calendar-year greg-date))))
    (while (>= date (calendar-absolute-from-hebrew (list 7 1 (1+ year))))
        (set! year (1+ year)))
    (let ((length (hebrew-calendar-last-month-of-year year)))
      (while (> date
                (calendar-absolute-from-hebrew
                 (list month
                       (hebrew-calendar-last-day-of-month month year)
                       year)))
	     (set! month (1+ (modulo month length)))))
    (set! day (1+
               (- date (calendar-absolute-from-hebrew (list month 1 year)))))
    (list month day year)))

(define (hebrew-calendar-leap-year? year)
  "#t if YEAR is a Hebrew calendar leap year."
  (< (modulo (1+ (* 7 year)) 19) 7))

(define (hebrew-calendar-last-month-of-year year)
  "The last month of the Hebrew calendar YEAR."
  (if (hebrew-calendar-leap-year? year)
      13
      12))

(define (hebrew-calendar-last-day-of-month month year)
  "The last day of MONTH in YEAR (in the Hebrew calendar)."
  (if (or (memq month (list 2 4 6 10 13))
          (and (= month 12) (not (hebrew-calendar-leap-year? year)))
          (and (= month 8) (not (hebrew-calendar-long-heshvan? year)))
          (and (= month 9) (hebrew-calendar-short-kislev? year)))
      29
      30))

(define (hebrew-calendar-elapsed-days year)
  "Days from Sun. prior to start of Hebrew calendar to mean conjunction of Tishri of Hebrew YEAR."
  (let* ((months-elapsed
          (+ (* 235 (quotient (1- year) 19));; Months in complete cycles so far.
             (* 12 (modulo (1- year) 19))      ;; Regular months in this cycle
             (quotient (1+ (* 7 (modulo (1- year) 19))) 19)));; Leap months this cycle
         (parts-elapsed (+ 204 (* 793 (modulo months-elapsed 1080))))
         (hours-elapsed (+ 5
                           (* 12 months-elapsed)
                           (* 793 (quotient months-elapsed 1080))
                           (quotient parts-elapsed 1080)))
         (parts                                  ;; Conjunction parts
          (+ (* 1080 (modulo hours-elapsed 24)) (modulo parts-elapsed 1080)))
         (day                                    ;; Conjunction day
          (+ 1 (* 29 months-elapsed) (quotient hours-elapsed 24)))
         (alternative-day
          (if (or (>= parts 19440)    ;; If the new moon is at or after midday,
                  (and (= (modulo day 7) 2);; ...or is on a Tuesday...
                       (>= parts 9924)  ;;    at 9 hours, 204 parts or later...
                       (not (hebrew-calendar-leap-year? year)));; of a
                                                                ;; common year,
                  (and (= (modulo day 7) 1);; ...or is on a Monday...
                       (>= parts 16789) ;;   at 15 hours, 589 parts or later...
                       (hebrew-calendar-leap-year? (1- year))));; at the end
                                                     ;; of a leap year
	      ;; Then postpone Rosh HaShanah one day
              (1+ day)
	      ;; Else
	      day)))
    (if ;; If Rosh HaShanah would occur on Sunday, Wednesday, or Friday
     (memq (modulo alternative-day 7) (list 0 3 5))
     ;; Then postpone it one (more) day and return
     (1+ alternative-day)
     ;; Else return
     alternative-day)))

(define (hebrew-calendar-days-in-year year)
  "Number of days in Hebrew YEAR."
  (- (hebrew-calendar-elapsed-days (1+ year))
     (hebrew-calendar-elapsed-days year)))

(define (hebrew-calendar-long-heshvan? year)
  "#t if Heshvan is long in Hebrew YEAR."
  (= (modulo (hebrew-calendar-days-in-year year) 10) 5))

(define (hebrew-calendar-short-kislev? year)
  "#t if Kislev is short in Hebrew YEAR."
  (= (modulo (hebrew-calendar-days-in-year year) 10) 3))

(define (calendar-absolute-from-hebrew date)
  "Day count corresponding to  Hebrew DATE."
  (let* ((month (extract-calendar-month date))
         (day (extract-calendar-day date))
         (year (extract-calendar-year date)))
    (+ day                            ;; Days so far this month.
       (if (< month 7);; before Tishri
	   ;; Then add days in prior months this year before and after Nisan
           (+ (let ((m 7)
		    (sum 0))
		(while (<= m (hebrew-calendar-last-month-of-year year))
		       (set! sum (+ sum (hebrew-calendar-last-day-of-month m year)))
		       (set! m (1+ m)))
		sum)
	      (let ((m 1)
		    (sum 0))
		(while (< m month)
		       (set! sum (+ sum (hebrew-calendar-last-day-of-month m year)))
		       (set! m (1+ m)))
		sum))
       ;; Else add days in prior months this year
       (let ((m 7)
	     (sum 0))
	 (while (< m month)
		(set! sum (+ sum (hebrew-calendar-last-day-of-month m year)))
		(set! m (1+ m)))
	 sum))
       (hebrew-calendar-elapsed-days year);; Days in prior years.
       -1373429)))                        ;; Days elapsed before absolute date 1.


;;; Chinese calendar

;; This collection of functions implements conversions to and from
;; Chinese calendar.  The rules used for the Chinese calendar are
;; those of Baolin Liu (see L. E. Doggett's article "Calendars" in the
;; Explanatory Supplement to the Astronomical Almanac, second edition,
;; 1992) for the calendar as revised at the beginning of the Qing
;; dynasty in 1644.  The nature of the astronomical calculations is
;; such that precise calculations cannot be made without great expense
;; in time, so that the calendars produced may not agree perfectly
;; with published tables--but no two pairs of published tables agree
;; perfectly either!  Liu's rules produce a calendar for 2033 which is
;; not accepted by all authorities.  The date of Chinese New Year is
;; correct from 1644-2051.

;; chinese-calendar-time-zone --- *Number of minutes difference
;; between local standard time for Chinese calendar and Coordinated
;; Universal (Greenwich) Time.  Default is for Beijing.  This is an
;; expression in `year' since it changed at 1928-01-01 00:00:00 from
;; UT+7:45:40 to UT+8.
(define chinese-calendar-time-zone
  '(if (< year 1928)
       (+ 465 (/ 40.0 60.0))
       480))

;; chinese-calendar-location-name --- Name of location used for
;; calculation of Chinese calendar
(define chinese-calendar-location-name "Beijing")

;; chinese-calendar-daylight-time-offset --- Number of minutes
;; difference between daylight savings and standard time for Chinese
;; calendar.  Default is no DST.  Should be 60, but DST is NOT used in
;; determining astronomical events.
(define chinese-calendar-daylight-time-offset 0)

;; chinese-calendar-standard-time-zone-name --- Abbreviated name of
;; standard time zone used for Chinese calendar.  This is an
;; expression depending on `year' because it changed at 1928-01-01
;; 00:00:00 from `PMT' to `CST'.
(define chinese-calendar-standard-time-zone-name
  '(if (< year 1928)
       "PMT"
       "CST"))

;; chinese-calendar-daylight-time-zone-name -- Abbreviated name of
;; daylight-savings time zone used for Chinese calendar
(define chinese-calendar-daylight-time-zone-name "CDT")

;; chinese-calendar-daylight-savings-starts --- The date on which
;; daylight savings time starts for Chinese calendar.  Default is for
;; no daylight savings time.  See documentation of
;; `calendar-daylight-savings-starts'.  The correct value is as
;; follows, but the Chinese calendrical authorities do NOT use DST in
;; determining astronomical events:
;;  (cond ((< 1986 year) (calendar-nth-named-day 1 0 4 year 10))
;;        ((= 1986 year) '(5 4 1986))
;;        (#t #f))
(define chinese-calendar-daylight-savings-starts #f)

;; chinese-calendar-daylight-savings-ends --- The date on which
;; daylight savings time ends for Chinese calendar.  Default is for no
;; daylight savings time.  See documentation of
;; `calendar-daylight-savings-ends'.  The correct value is as follows,
;; but the Chinese calendrical authorities do NOT use DST in
;; determining astronomical events:
;; (if (<= 1986 year) (calendar-nth-named-day 1 0 9 year 11))
(define chinese-calendar-daylight-savings-ends #f)

;; chinese-calendar-daylight-savings-starts-time --- Number of minutes
;; after midnight that daylight savings time starts for Chinese
;; calendar.  Default is for no daylight savings time.
(define chinese-calendar-daylight-savings-starts-time 0)

;; chinese-calendar-daylight-savings-ends-time --- Number of minutes
;; after midnight that daylight savings time ends for Chinese
;; calendar.  Default is for no daylight savings time.
(define chinese-calendar-daylight-savings-ends-time 0)

(define (chinese-zodiac-sign-on-or-after d)
  "Absolute date of first new Zodiac sign on or after absolute date d.
The Zodiac signs begin when the sun's longitude is a multiple of 30 degrees."
  (let* ((year (extract-calendar-year
                (calendar-gregorian-from-absolute d)))
         (calendar-time-zone (eval chinese-calendar-time-zone))
         (calendar-daylight-time-offset
          chinese-calendar-daylight-time-offset)
         (calendar-standard-time-zone-name
          chinese-calendar-standard-time-zone-name)
         (calendar-daylight-time-zone-name
          chinese-calendar-daylight-time-zone-name)
         (calendar-calendar-daylight-savings-starts
          chinese-calendar-daylight-savings-starts)
         (calendar-daylight-savings-ends
          chinese-calendar-daylight-savings-ends)
         (calendar-daylight-savings-starts-time
          chinese-calendar-daylight-savings-starts-time)
         (calendar-daylight-savings-ends-time
          chinese-calendar-daylight-savings-ends-time))
    (inexact->exact (floor
		     (calendar-absolute-from-astro
		      (solar-date-next-longitude
		       (calendar-astro-from-absolute d)
		       30))))))

(define (chinese-new-moon-on-or-after d)
  "Absolute date of first new moon on or after absolute date d."
  (let* ((year (extract-calendar-year
                (calendar-gregorian-from-absolute d)))
         (calendar-time-zone (eval chinese-calendar-time-zone))
         (calendar-daylight-time-offset
          chinese-calendar-daylight-time-offset)
         (calendar-standard-time-zone-name
          chinese-calendar-standard-time-zone-name)
         (calendar-daylight-time-zone-name
          chinese-calendar-daylight-time-zone-name)
         (calendar-calendar-daylight-savings-starts
          chinese-calendar-daylight-savings-starts)
         (calendar-daylight-savings-ends
          chinese-calendar-daylight-savings-ends)
         (calendar-daylight-savings-starts-time
          chinese-calendar-daylight-savings-starts-time)
         (calendar-daylight-savings-ends-time
          chinese-calendar-daylight-savings-ends-time))
    (inexact->exact (floor
		     (calendar-absolute-from-astro
		      (lunar-new-moon-on-or-after
		       (calendar-astro-from-absolute d)))))))

;; chinese-year-cache --- An assoc list of Chinese year structures as
;; determined by `chinese-year'.
;;
;; Values are computed as needed, but to save time, the initial value consists
;; of the precomputed years 1990-2010.  The code works just as well with this
;; set to nil initially (which is how the value for 1990-2010 was computed).

(define chinese-year-cache
  '((1990 (12 726464) (1 726494) (2 726523) (3 726553) (4 726582) (5 726611)
          (5.5 726641) (6 726670) (7 726699) (8 726729) (9 726758) (10 726788)
          (11 726818))
    (1991 (12 726848) (1 726878) (2 726907) (3 726937) (4 726966) (5 726995)
          (6 727025) (7 727054) (8 727083) (9 727113) (10 727142) (11 727172))
    (1992 (12 727202) (1 727232) (2 727261) (3 727291) (4 727321) (5 727350)
          (6 727379) (7 727409) (8 727438) (9 727467) (10 727497) (11 727526))
    (1993 (12 727556) (1 727586) (2 727615) (3 727645) (3.5 727675) (4 727704)
          (5 727734) (6 727763) (7 727793) (8 727822) (9 727851) (10 727881)
          (11 727910))
    (1994 (12 727940) (1 727969) (2 727999) (3 728029) (4 728059) (5 728088)
          (6 728118) (7 728147) (8 728177) (9 728206) (10 728235) (11 728265))
    (1995 (12 728294) (1 728324) (2 728353) (3 728383) (4 728413) (5 728442)
          (6 728472) (7 728501) (8 728531) (8.5 728561) (9 728590) (10 728619)
          (11 728649))
    (1996 (12 728678) (1 728708) (2 728737) (3 728767) (4 728796) (5 728826)
          (6 728856) (7 728885) (8 728915) (9 728944) (10 728974) (11 729004))
    (1997 (12 729033) (1 729062) (2 729092) (3 729121) (4 729151) (5 729180)
          (6 729210) (7 729239) (8 729269) (9 729299) (10 729328) (11 729358))
    (1998 (12 729388) (1 729417) (2 729447) (3 729476) (4 729505) (5 729535)
          (5.5 729564) (6 729593) (7 729623) (8 729653) (9 729682) (10 729712)
          (11 729742))
    (1999 (12 729771) (1 729801) (2 729831) (3 729860) (4 729889) (5 729919)
          (6 729948) (7 729977) (8 730007) (9 730036) (10 730066) (11 730096))
    (2000 (12 730126) (1 730155) (2 730185) (3 730215) (4 730244) (5 730273)
          (6 730303) (7 730332) (8 730361) (9 730391) (10 730420) (11 730450))
    (2001 (12 730480) (1 730509) (2 730539) (3 730569) (4 730598) (4.5 730628)
          (5 730657) (6 730687) (7 730716) (8 730745) (9 730775) (10 730804)
          (11 730834))
    (2002 (12 730863) (1 730893) (2 730923) (3 730953) (4 730982) (5 731012)
          (6 731041) (7 731071) (8 731100) (9 731129) (10 731159) (11 731188))
    (2003 (12 731218) (1 731247) (2 731277) (3 731307) (4 731336) (5 731366)
          (6 731396) (7 731425) (8 731455) (9 731484) (10 731513) (11 731543))
    (2004 (12 731572) (1 731602) (2 731631) (2.5 731661) (3 731690) (4 731720)
          (5 731750) (6 731779) (7 731809) (8 731838) (9 731868) (10 731897)
          (11 731927))
    (2005 (12 731956) (1 731986) (2 732015) (3 732045) (4 732074) (5 732104)
          (6 732133) (7 732163) (8 732193) (9 732222) (10 732252) (11 732281))
    (2006 (12 732311) (1 732340) (2 732370) (3 732399) (4 732429) (5 732458)
          (6 732488) (7 732517) (7.5 732547) (8 732576) (9 732606) (10 732636)
          (11 732665))
    (2007 (12 732695) (1 732725) (2 732754) (3 732783) (4 732813) (5 732842)
          (6 732871) (7 732901) (8 732930) (9 732960) (10 732990) (11 733020))
    (2008 (12 733049) (1 733079) (2 733109) (3 733138) (4 733167) (5 733197)
          (6 733226) (7 733255) (8 733285) (9 733314) (10 733344) (11 733374))
    (2009 (12 733403) (1 733433) (2 733463) (3 733493) (4 733522) (5 733551)
          (5.5 733581) (6 733610) (7 733639) (8 733669) (9 733698) (10 733728)
          (11 733757))
    (2010 (12 733787) (1 733817) (2 733847) (3 733876) (4 733906) (5 733935)
          (6 733965) (7 733994) (8 734023) (9 734053) (10 734082) (11 734112))))

;; chinese-year --- The structure of the Chinese year for Gregorian
;; year Y.  The result is a list of pairs (i d), where month i begins
;; on absolute date d, of the Chinese months from the Chinese month
;; following the solstice in Gregorian year Y-1 to the Chinese month
;; of the solstice of Gregorian year Y.  The list is cached for
;; further use.
(define (chinese-year y)
  (let ((list (cdr (assoc y chinese-year-cache))))
    (if (null? list)
        (begin
          (set! list (compute-chinese-year y))
          (set! chinese-year-cache
                (append chinese-year-cache (list (cons y list))))))
    list))

;; number-chinese-months --- Assign month numbers to the lunar months
;; in LIST, starting with START.  Numbers are assigned sequentially,
;; START, START+1, ..., 11, with half numbers used for leap months.
;; First month of list will never be a leap month, nor will the last.
(define (number-chinese-months list start)
  (if (not (null? list))
      (if (zero? (- 12 start (length list)))
          ;; List is too short for a leap month
          (cons (list start (car list))
                (number-chinese-months (cdr list) (1+ start)))
	  (cons
	   ;; First month
	   (list start (car list))
	   ;; Remaining months
	   (if (and (cdr (cdr list));; at least two more months...
		    (<= (car (cdr (cdr list)))
			(chinese-zodiac-sign-on-or-after (car (cdr list)))))
	       ;; Next month is a leap month
	       (cons (list (+ start 0.5) (car (cdr list)))
		     (number-chinese-months (cdr (cdr list)) (1+ start)))
	       ;; Next month is not a leap month
	       (number-chinese-months (cdr list) (1+ start)))))))

;; chinese-month-list --- List of starting dates of Chinese months
;; from START to END.
(define (chinese-month-list start end)
  (if (<= start end)
      (let ((new-moon (chinese-new-moon-on-or-after start)))
        (if (<= new-moon end)
            (cons new-moon
                  (chinese-month-list (1+ new-moon) end))))))

;; compute-chinese-year --- Compute the structure of the Chinese year
;; for Gregorian year Y.  The result is a list of pairs (i d), where
;; month i begins on absolute date d, of the Chinese months from the
;; Chinese month following the solstice in Gregorian year Y-1 to the
;; Chinese month of the solstice of Gregorian year Y.
(define (compute-chinese-year y)
  (let* ((next-solstice (chinese-zodiac-sign-on-or-after
                         (calendar-absolute-from-gregorian
                          (list 12 15 y))))
         (list (chinese-month-list (1+ (chinese-zodiac-sign-on-or-after
                                        (calendar-absolute-from-gregorian
                                         (list 12 15 (1- y)))))
                                   next-solstice))
         (next-sign (chinese-zodiac-sign-on-or-after (car list))))
    (if (= (length list) 12)
        ;; No room for a leap month, just number them 12, 1, 2, ..., 11
        (cons (list 12 (car list))
              (number-chinese-months (cdr list) 1))
	;; Now we can assign numbers to the list for y
	;; The first month or two are special
	(if (or (> (car list) next-sign) (>= next-sign (car (cdr list))))
	    ;; First month on list is a leap month, second is not
	    (append (list (list 11.5 (car list))
			  (list 12 (car (cdr list))))
		    (number-chinese-months (cdr (cdr list)) 1))
	    ;; First month on list is not a leap month
	    (append (list (list 12 (car list)))
		    (if (>= (chinese-zodiac-sign-on-or-after (car (cdr list)))
			    (car (cdr (cdr list))))
			;; Second month on list is a leap month
			(cons (list 12.5 (car (cdr list)))
			      (number-chinese-months (cdr (cdr list)) 1))
			;; Second month on list is not a leap month
			(number-chinese-months (cdr list) 1)))))))

;; calendar-absolute-from-chinese --- The number of days elapsed
;; between the Gregorian date 12/31/1 BC and DATE.  The Gregorian date
;; Sunday, December 31, 1 BC is imaginary.
(define (calendar-absolute-from-chinese date)
  (let* ((cycle (car date))
         (year (car (cdr date)))
         (month (car (cdr (cdr date))))
         (day (car (cdr (cdr (cdr date)))))
         (g-year (+ (* (1- cycle) 60);; years in prior cycles
                    (1- year)        ;; prior years this cycle
                    -2636)))         ;; years before absolute date 0
    (+ (1- day);; prior days this month
       (car
        (cdr    ;; absolute date of start of this month
         (assoc month (append (memq (assoc 1 (chinese-year g-year))
                                    (chinese-year g-year))
                              (chinese-year (1+ g-year)))))))))

;; calendar-chinese-from-absolute --- Compute Chinese date (cycle year
;; month day) corresponding to absolute DATE.  The absolute date is
;; the number of days elapsed since the (imaginary) Gregorian date
;; Sunday, December 31, 1 BC.
(define (calendar-chinese-from-absolute date)
  (let* ((g-year (extract-calendar-year
                  (calendar-gregorian-from-absolute date)))
         (c-year (+ g-year 2695))
         (ylist (append (chinese-year (1- g-year))
                       (chinese-year g-year)
                       (chinese-year (1+ g-year)))))
    (while (<= (car (cdr (car (cdr ylist)))) date)
	   ;; the first month on the list is in Chinese year c-year
	   ;; date is on or after start of second month on list...
	   (if (= 1 (car (car (cdr ylist))))
	       ;; second month on list is a new Chinese year
	       (set! c-year (1+ c-year)))
	   ;; ...so first month on list is of no interest
	   (set! ylist (cdr ylist)))
    (list (quotient (1- c-year) 60)
          (calendar-mod c-year 60)
          (car (car ylist))
          (1+ (- date (car (cdr (car ylist))))))))

;; chinese-months --- A list of the months in cycle C, year Y of the
;; Chinese calendar.
(define (chinese-months c y)
  (let* ((l (memq 1 (append
                     (mapcar '(lambda (x)
                                (car x))
                             (chinese-year (extract-calendar-year
                                            (calendar-gregorian-from-absolute
                                             (calendar-absolute-from-chinese
                                              (list c y 1 1))))))
                     (mapcar '(lambda (x)
                                (if (> (car x) 11) (car x)))
                             (chinese-year (extract-calendar-year
                                            (calendar-gregorian-from-absolute
                                             (calendar-absolute-from-chinese
                                              (list (if (= y 60) (1+ c) c)
                                                    (if (= y 60) 1 y)
                                                    1 1))))))))))
    l))

;; make-chinese-month-assoc-list --- Make list of months L into an
;; assoc list.
;; NOTE: I need to figure out how the format works.
(define (make-chinese-month-assoc-list l)
  (if (and (not (null? l)) (car l))
      (if (and (not (null? (cdr l))) (car (cdr l)))
          (if (= (car l) (floor (car (cdr l))))
              (append
               (list (cons (format "%s (first)" (car l)) (car l))
                     (cons (format "%s (second)" (car l)) (car (cdr l))))
               (make-chinese-month-assoc-list (cdr (cdr l))))
	      (append
	       (list (cons (int-to-string (car l)) (car l)))
	       (make-chinese-month-assoc-list (cdr l))))
	  (list (cons (int-to-string (car l)) (car l))))))


;;; Lunar Calculations

;; Based on ``Astronomical Formulae for Calculators,'' 3rd ed., by
;; Jean Meeus, Willmann-Bell, Inc., 1985 and ``Astronomical
;; Algorithms'' by Jean Meeus, Willmann-Bell, Inc., 1991.
;;
;; WARNING: The calculations will be accurate only to within a few minutes.

;; The author [Reingold (but I too)] would be delighted to have an
;; astronomically more sophisticated person rewrite the code for the
;; lunar calculations in this file!


(define (lunar-phase-list month year)
  "List of lunar phases for three months starting with Gregorian MONTH, YEAR"
  (let ((end-month month)
        (end-year year)
        (start-month month)
        (start-year year))
    (increment-calendar-month end-month end-year 3)
    (increment-calendar-month start-month start-year -1)
    (let* ((end-date (list (list end-month 1 end-year)))
           (start-date (list (list start-month 
                                   (calendar-last-day-of-month
                                    start-month start-year)
                                   start-year)))
           (index (* 4
                     (truncate
                      (* 12.3685
                         (+ year
                            ( / (calendar-day-number (list month 1 year))
                                366.0)
                            -1900)))))
           (new-moon (lunar-phase index))
           (list))
      (while (calendar-date-compare new-moon end-date)
	     (if (calendar-date-compare start-date new-moon)
		 (set! list (append list (list new-moon))))
	     (set! index (1+ index))
	     (set! new-moon (lunar-phase index)))
      list)))

;; lunar-phase --- Local date and time of lunar phase INDEX.  Integer
;; below INDEX/4 gives the lunation number, counting from Jan 1, 1900;
;; remainder mod 4 gives the phase: 0 new moon, 1 first quarter, 2
;; full moon, 3 last quarter.
(define (lunar-phase index)
  "Local date and time of lunar phase INDEX"
  (let* ((phase (modulo index 4))
         (index (/ index 4.0))
         (time (/ index 1236.85))
         (date (+ (calendar-absolute-from-gregorian '(1 0.5 1900))
                  0.75933
                  (* 29.53058868 index)
                  (* 0.0001178 time time)
                  (* -0.000000155 time time time)
                  (* 0.00033
                     (solar-sin-degrees (+ 166.56
                                           (* 132.87 time)
                                           (* -0.009173 time time))))))
         (sun-anomaly (mod
                       (+ 359.2242
                          (* 29.105356 index)
                          (* -0.0000333 time time)
                          (* -0.00000347 time time time))
                       360.0))
         (moon-anomaly (mod
                        (+ 306.0253
                           (* 385.81691806 index)
                           (* 0.0107306 time time)
                           (* 0.00001236 time time time))
                        360.0))
         (moon-lat (mod
                    (+ 21.2964
                       (* 390.67050646 index)
                       (* -0.0016528 time time)
                       (* -0.00000239 time time time))
                    360.0))
         (adjustment
          (if (memq phase '(0 2))
              (+ (* (- 0.1734 (* 0.000393 time))
                    (solar-sin-degrees sun-anomaly))
                 (* 0.0021 (solar-sin-degrees (* 2 sun-anomaly)))
                 (* -0.4068 (solar-sin-degrees moon-anomaly))
                 (* 0.0161 (solar-sin-degrees (* 2 moon-anomaly)))
                 (* -0.0004 (solar-sin-degrees (* 3 moon-anomaly)))
                 (* 0.0104 (solar-sin-degrees (* 2 moon-lat)))
                 (* -0.0051 (solar-sin-degrees (+ sun-anomaly moon-anomaly)))
                 (* -0.0074 (solar-sin-degrees (- sun-anomaly moon-anomaly)))
                 (* 0.0004 (solar-sin-degrees (+ (* 2 moon-lat) sun-anomaly)))
                 (* -0.0004 (solar-sin-degrees (- (* 2 moon-lat) sun-anomaly)))
                 (* -0.0006 (solar-sin-degrees
                             (+ (* 2 moon-lat) moon-anomaly)))
                 (* 0.0010 (solar-sin-degrees (- (* 2 moon-lat) moon-anomaly)))
                 (* 0.0005 (solar-sin-degrees
                            (+ (* 2 moon-anomaly) sun-anomaly))))
	      (+ (* (- 0.1721 (* 0.0004 time))
		    (solar-sin-degrees sun-anomaly))
		 (* 0.0021 (solar-sin-degrees (* 2 sun-anomaly)))
		 (* -0.6280 (solar-sin-degrees moon-anomaly))
		 (* 0.0089 (solar-sin-degrees (* 2 moon-anomaly)))
		 (* -0.0004 (solar-sin-degrees (* 3 moon-anomaly)))
		 (* 0.0079 (solar-sin-degrees (* 2 moon-lat)))
		 (* -0.0119 (solar-sin-degrees (+ sun-anomaly moon-anomaly)))
		 (* -0.0047 (solar-sin-degrees (- sun-anomaly moon-anomaly)))
		 (* 0.0003 (solar-sin-degrees (+ (* 2 moon-lat) sun-anomaly)))
		 (* -0.0004 (solar-sin-degrees (- (* 2 moon-lat) sun-anomaly)))
		 (* -0.0006 (solar-sin-degrees (+ (* 2 moon-lat) moon-anomaly)))
		 (* 0.0021 (solar-sin-degrees (- (* 2 moon-lat) moon-anomaly)))
		 (* 0.0003 (solar-sin-degrees
			    (+ (* 2 moon-anomaly) sun-anomaly)))
		 (* 0.0004 (solar-sin-degrees
			    (- sun-anomaly (* 2 moon-anomaly))))
		 (* -0.0003 (solar-sin-degrees
			     (+ (* 2 sun-anomaly) moon-anomaly))))))
         (adj (+ 0.0028
                 (* -0.0004 (solar-cosine-degrees
                             sun-anomaly))
                 (* 0.0003 (solar-cosine-degrees
                            moon-anomaly))))
         (adjustment (cond ((= phase 1) (+ adjustment adj))
                           ((= phase 2) (- adjustment adj))
                           (#t adjustment)))
         (date (+ date adjustment))
	 (date (+ date (/ (- calendar-time-zone
			     (solar-ephemeris-correction
                              (extract-calendar-year
                               (calendar-gregorian-from-absolute
                                (inexact->exact (truncate date))))))
			  60.0 24.0)))
         (time (* 24 (- date (truncate date))))
	 (date (calendar-gregorian-from-absolute (inexact->exact (truncate date))))
         (adj (dst-adjust-time date time)))
    (list (car adj) (apply 'solar-time-string (cdr adj)) phase)))

;;  For the Chinese calendar the calculations for the new moon need to be more
;;  accurate than those above, so we use more terms in the approximation.

;; lunar-new-moon-time --- Astronomical (Julian) day number of K th
;; new moon.
(define (lunar-new-moon-time k)
  "Astronomical (Julian) day number of K th new moon"
  (let* ((T (/ k 1236.85))
	 (T2 (* T T))
	 (T3 (* T T T))
	 (T4 (* T2 T2))
	 (JDE (+ 2451550.09765
		 (* 29.530588853 k)
		 (* 0.0001337 T2)
		 (* -0.000000150 T3)
		 (* 0.00000000073 T4)))
	 (E (- 1 (* 0.002516 T) (* 0.0000074 T2)))
	 (sun-anomaly (+ 2.5534
			 (* 29.10535669 k)
			 (* -0.0000218 T2)
			 (* -0.00000011 T3)))
	 (moon-anomaly (+ 201.5643
			  (* 385.81693528 k)
			  (* 0.0107438 T2)
			  (* 0.00001239 T3)
			  (* -0.000000058 T4)))
	 (moon-argument (+ 160.7108
			   (* 390.67050274 k)
			   (* -0.0016341 T2)
			   (* -0.00000227 T3)
			   (* 0.000000011 T4)))
	 (omega (+ 124.7746
		   (* -1.56375580 k)
		   (* 0.0020691 T2)
		   (* 0.00000215 T3)))
	 (A1  (+ 299.77 (*  0.107408 k) (* -0.009173 T2)))
	 (A2  (+ 251.88 (*  0.016321 k)))
	 (A3  (+ 251.83 (* 26.641886 k)))
	 (A4  (+ 349.42 (* 36.412478 k)))
	 (A5  (+  84.66 (* 18.206239 k)))
	 (A6  (+ 141.74 (* 53.303771 k)))
	 (A7  (+ 207.14 (*  2.453732 k)))
	 (A8  (+ 154.84 (*  7.306860 k)))
	 (A9  (+  34.52 (* 27.261239 k)))
	 (A10 (+ 207.19 (*  0.121824 k)))
	 (A11 (+ 291.34 (*  1.844379 k)))
	 (A12 (+ 161.72 (* 24.198154 k)))
	 (A13 (+ 239.56 (* 25.513099 k)))
	 (A14 (+ 331.55 (*  3.592518 k)))
	 (correction
	    (+ (* -0.40720   (solar-sin-degrees moon-anomaly))
	       (*  0.17241 E (solar-sin-degrees sun-anomaly))
	       (*  0.01608   (solar-sin-degrees (* 2 moon-anomaly)))
	       (*  0.01039   (solar-sin-degrees (* 2 moon-argument)))
	       (*  0.00739 E (solar-sin-degrees (- moon-anomaly sun-anomaly)))
	       (* -0.00514 E (solar-sin-degrees (+ moon-anomaly sun-anomaly)))
	       (*  0.00208 E E (solar-sin-degrees (* 2 sun-anomaly)))
	       (* -0.00111   (solar-sin-degrees
			       (- moon-anomaly (* 2 moon-argument))))
	       (* -0.00057   (solar-sin-degrees
			       (+ moon-anomaly (* 2 moon-argument))))
	       (*  0.00056 E (solar-sin-degrees
			       (+ (* 2 moon-anomaly) sun-anomaly)))
	       (* -0.00042   (solar-sin-degrees (* 3 moon-anomaly)))
	       (*  0.00042 E (solar-sin-degrees
			       (+ sun-anomaly (* 2 moon-argument))))
	       (*  0.00038 E (solar-sin-degrees
			       (- sun-anomaly (* 2 moon-argument))))
	       (* -0.00024 E (solar-sin-degrees
			       (- (* 2 moon-anomaly) sun-anomaly)))
	       (* -0.00017   (solar-sin-degrees omega))
	       (* -0.00007   (solar-sin-degrees
			       (+ moon-anomaly (* 2 sun-anomaly))))
	       (*  0.00004   (solar-sin-degrees
			       (- (* 2 moon-anomaly) (* 2 moon-argument))))
	       (*  0.00004   (solar-sin-degrees (* 3 sun-anomaly)))
	       (*  0.00003   (solar-sin-degrees (+ moon-anomaly sun-anomaly
						   (* -2 moon-argument))))
	       (*  0.00003   (solar-sin-degrees
			       (+ (* 2 moon-anomaly) (* 2 moon-argument))))
	       (* -0.00003   (solar-sin-degrees (+ moon-anomaly sun-anomaly
						   (* 2 moon-argument))))
	       (*  0.00003   (solar-sin-degrees (- moon-anomaly sun-anomaly
						   (* -2 moon-argument))))
	       (* -0.00002   (solar-sin-degrees (- moon-anomaly sun-anomaly
						   (* 2 moon-argument))))
	       (* -0.00002   (solar-sin-degrees
			       (+ (* 3 moon-anomaly) sun-anomaly)))
	       (*  0.00002   (solar-sin-degrees (* 4 moon-anomaly)))))
	 (additional
	    (+ (* 0.000325 (solar-sin-degrees A1))
	       (* 0.000165 (solar-sin-degrees A2))
	       (* 0.000164 (solar-sin-degrees A3))
	       (* 0.000126 (solar-sin-degrees A4))
	       (* 0.000110 (solar-sin-degrees A5))
	       (* 0.000062 (solar-sin-degrees A6))
	       (* 0.000060 (solar-sin-degrees A7))
	       (* 0.000056 (solar-sin-degrees A8))
	       (* 0.000047 (solar-sin-degrees A9))
	       (* 0.000042 (solar-sin-degrees A10))
	       (* 0.000040 (solar-sin-degrees A11))
	       (* 0.000037 (solar-sin-degrees A12))
	       (* 0.000035 (solar-sin-degrees A13))
	       (* 0.000023 (solar-sin-degrees A14))))
	 (newJDE (+ JDE correction additional)))
    (+ newJDE
       (- (solar-ephemeris-correction
           (extract-calendar-year
            (calendar-gregorian-from-absolute
             (ineaxact->exact (floor (calendar-absolute-from-astro newJDE)))))))
       (/ calendar-time-zone 60.0 24.0))))

;; lunar-new-moon-on-or-after --- Astronomical (Julian) day number of
;; first new moon on or after astronomical (Julian) day number d.  The
;; fractional part is the time of day.
;;
;; The date and time are local time, including any daylight savings
;; rules, as governed by the values of
;; calendar-daylight-savings-starts,
;; calendar-daylight-savings-starts-time,
;; calendar-daylight-savings-ends,
;; calendar-daylight-savings-ends-time, calendar-daylight-time-offset,
;; and calendar-time-zone.
(define (lunar-new-moon-on-or-after d)
  "Astronomical (Julian) day number of first new moon on or after astronomical (Julian) day number D"
  (let* ((date (calendar-gregorian-from-absolute
                (inexact->exact (floor (calendar-absolute-from-astro d)))))
         (year (+ (extract-calendar-year date)
		  (/ (calendar-day-number date) 365.25)))
	 (k (inexact->exact (floor (* (- year 2000.0) 12.3685))))
         (date (lunar-new-moon-time k)))
    (while (< date d)
	   (set! k (1+ k))
	   (set! date (lunar-new-moon-time k)))
    (let* ((a-date (calendar-absolute-from-astro date))
           (time (* 24 (- a-date (truncate a-date))))
           (date (calendar-gregorian-from-absolute (inexact->exact (truncate a-date))))
           (adj (dst-adjust-time date time)))
      (calendar-astro-from-absolute
       (+ (calendar-absolute-from-gregorian (car adj))
          (/ (car (cdr adj)) 24.0))))))


;;; Solar calculations

;; This collection of functions deals with times of day,
;; sunrise/sunset, and equinoxes/solstices.

;; Based on the ``Almanac for Computers 1984,'' prepared by the
;; Nautical Almanac Office, United States Naval Observatory,
;; Washington, 1984, on ``Astronomical Formulae for Calculators,'' 3rd
;; ed., by Jean Meeus, Willmann-Bell, Inc., 1985, on ``Astronomical
;; Algorithms'' by Jean Meeus, Willmann-Bell, Inc., 1991, and on
;; ``Planetary Programs and Tables from -4000 to +2800'' by Pierre
;; Bretagnon and Jean-Louis Simon, Willmann-Bell, Inc., 1986.

;;
;; Accuracy:
;;    1. Sunrise/sunset times will be accurate to the minute for years
;;       1951--2050.  For other years the times will be within +/- 2 minutes.
;;
;;    2. Equinox/solstice times will be accurate to the minute for years
;;       1951--2050.  For other years the times will be within +/- 1 minute.

;; solar-sidereal-time-greenwich-midnight --- Sidereal time at
;; Greenwich at midnight (universal time).
(define solar-sidereal-time-greenwich-midnight #f)

;; solar-spring-or-summer-season --- T if spring or summer and #f
;; otherwise.  Needed for polar areas, in order to know whether the
;; day lasts 0 or 24 hours.
(define solar-spring-or-summer-season #f)


;; NOTE: I dropped the (condition-case) protection that the original
;; had to cover arithmetic exceptions on some machines.  I assume that
;; this is not a problem in Guile.

(define (solar-sin-degrees x)
  (sin (degrees-to-radians (mod x 360.0))))
(define (solar-cosine-degrees x)
  (cos (degrees-to-radians (mod x 360.0))))
(define (solar-tangent-degrees x)
  (tan (degrees-to-radians (mod x 360.0))))

;; solar-xy-to-quadrant --- Determines the quadrant of the point X, Y.
(define (solar-xy-to-quadrant x y)
  "Determines the quadrant of the point X, Y"
  (if (> x 0)
      (if (> y 0) 1 4)
      (if (> y 0) 2 3)))

;; solar-degrees-to-quadrant --- Determines the quadrant of ANGLE.
(define (solar-degrees-to-quadrant angle)
  "Quadrant of ANGLE"
  (1+ (inexact->exact (floor (/ (mod angle 360) 90)))))

;; solar-arctan --- Arctangent of X in quadrant QUAD.
(define (solar-arctan x quad)
  "Arctangent of X in quadrant QUAD"
  (let ((deg (radians-to-degrees (atan x))))
    (cond ((equal? quad 2)   (+ deg 180))
	  ((equal? quad 3)   (+ deg 180))
	  ((equal? quad 4)   (+ deg 360))
	  (#t                deg))))

;; solar-atn2 --- Arctan of point X, Y.
(define (solar-atn2 x y)
  "Arctangent of point X, Y"
  (if (= x 0)
      (if (> y 0) 90 270)
      (solar-arctan (/ y x) x)))

;; solar-arccos --- Arcos of X.
(define (solar-arccos x)
  "Arccos of X"
  (let ((y (sqrt (- 1 (* x x)))))
    (solar-atn2 x y)))

;; solar-arcsin --- Arcsin of Y.
(define (solar-arcsin y)
  "Arcsin of Y"
  (let ((x (sqrt (- 1 (* y y)))))
    (solar-atn2 x y)
    ))

;; solar-degrees-to-hours --- Convert DEGREES to hours.
(define (solar-degrees-to-hours degrees)
  "Convert DEGREES to hours"
  (/ degrees 15.0))

;; solar-hours-to-days --- Convert HOUR to decimal fraction of a day.
(define (solar-hours-to-days hour)
  "Convert HOUR to days"
  (/ hour 24.0))

;; solar-right-ascension --- Right ascension of the sun, in hours,
;; given LONGITUDE and OBLIQUITY.  Both arguments are in degrees.
(define (solar-right-ascension longitude obliquity)
  "Right ascension of the sun, in hours, given LONGITUDE and OBLIQUITY in degrees"
  (solar-degrees-to-hours
   (solar-arctan
    (* (solar-cosine-degrees obliquity) (solar-tangent-degrees longitude))
    (solar-degrees-to-quadrant longitude))))

;; solar-declination --- Declination of the sun, in degrees, given
;; LONGITUDE and OBLIQUITY.  Both arguments are in degrees.
(define (solar-declination longitude obliquity)
  "Declination of the sun, in degrees, given LONGITUDE and OBLIQUITY in degrees"
  (solar-arcsin
   (* (solar-sin-degrees obliquity)
      (solar-sin-degrees longitude))))

;; solar-sunrise-and-sunset --- Sunrise, sunset and length of day.
;; Parameters are the midday TIME and the LATITUDE, LONGITUDE of the
;; location.
;;
;; TIME is a pair with the first component being the number of Julian
;; centuries elapsed at 0 Universal Time, and the second component
;; being the universal time.  For instance, the pair corresponding to
;; November 28, 1995 at 16 UT is \(-0.040945 16), -0.040945 being the
;; number of julian centuries elapsed between Jan 1, 2000 at 12 UT and
;; November 28, 1995 at 0 UT.
;;
;; Coordinates are included because this function is called with
;; latitude=10 degrees to find out if polar regions have 24 hours of
;; sun or only night.
(define (solar-sunrise-and-sunset time latitude longitude)
  "List of (sunrise, sunset, length) given TIME (century since 2000, ut), LATITUDE, and LONGITUDE"
  (let* ((rise-time (solar-moment -1 latitude longitude time))
         (set-time (solar-moment 1 latitude longitude time))
         (day-length 0))
    (if (not (and rise-time set-time 0))
        (if (or (and (> latitude 0) solar-spring-or-summer-season)
                (and (< latitude 0) (not solar-spring-or-summer-season)))
	    (set! day-length 24)
	    (set! day-length 0))
        (set! day-length (- set-time rise-time)))
    (list (if rise-time (+ rise-time (/ calendar-time-zone 60.0)) #f)
          (if set-time (+ set-time (/ calendar-time-zone 60.0)) #f)
          day-length)))

;; solar-moment --- Sunrise/sunset at location.  Sunrise if DIRECTION
;; =-1 or sunset if =1 at LATITUDE, LONGITUDE, with midday being TIME.
;;
;; TIME is a pair with the first component being the number of Julian
;; centuries elapsed at 0 Universal Time, and the second component
;; being the universal time.  For instance, the pair corresponding to
;; November 28, 1995 at 16 UT is \(-0.040945 16), -0.040945 being the
;; number of julian centuries elapsed between Jan 1, 2000 at 12 UT and
;; November 28, 1995 at 0 UT.
;;
;; Uses binary search.
(define (solar-moment direction latitude longitude time)
  "Sunrise or sunset (DIRECTION=-1 or 1) given midday TIME at LATITUDE, LONGITUDE"
  (let* ((ut (car (cdr time)))
         (possible 1) ; we assume that rise or set are possible
         (utmin (+ ut (* direction 12.0))) 
         (utmax ut)    ; the time searched is between utmin and utmax
					; utmin and utmax are in hours
         (utmoment-old 0.0)    ; rise or set approximation
         (utmoment 1.0) ; rise or set approximation
         (hut 0)         ; sun height at utmoment
         (t0 (car time))
         (hmin (car (cdr 
		     (solar-horizontal-coordinates (list t0 utmin) 
						   latitude longitude #t))))
         (hmax (car (cdr 
		     (solar-horizontal-coordinates (list t0 utmax) 
						   latitude longitude #t)))))
					; -0.61 degrees is the height of the middle of the sun, when it rises
					;   or sets.
    (if (< hmin -0.61) 
	(if (> hmax -0.61)
	    (while ;(< i 20) ; we perform a simple dichotomy
					; (> (abs (+ hut 0.61)) epsilon)
	     (>= (abs (- utmoment utmoment-old))
		 (/ solar-error 60))
	     (set! utmoment-old utmoment)
	     (set! utmoment (/ (+ utmin utmax) 2))
	     (set! hut (car (cdr 
			     (solar-horizontal-coordinates 
			      (list t0 utmoment) latitude longitude #t))))
	     (if (< hut -0.61) (set! utmin utmoment))
	     (if (> hut -0.61) (set! utmax utmoment))
	     )
	    (set! possible 0)) ; the sun never rises
	(set! possible 0)) ; the sun never sets
    (if (equal? possible 0) #f utmoment)))

;; solar-exact-local-noon --- Date and Universal Time of local noon at
;; *local date* date.
;;
;; The date may be different from the one asked for, but it will be
;; the right local date.  The second component of date should be an
;; integer.
(define (solar-exact-local-noon date)
  "Date and Universal Time of local noon on local date DATE"
  (let* ((nd date)
         (ut (- 12.0 (/ calendar-longitude 15)))
         (te (solar-time-equation date ut)))
    (set! ut (- ut te))
    (if (>= ut 24)
        (begin 
          (set! nd (list (car date) (+ 1 (car (cdr date)))
                         (car (cdr (cdr date)))))
          (set! ut (- ut 24))))
    (if (< ut 0)
        (begin 
          (set! nd (list (car date) (- (car (cdr date)) 1)
                         (car (cdr (cdr date)))))
          (set! ut (+ ut 24))))
    (set! nd (calendar-gregorian-from-absolute
	      (calendar-absolute-from-gregorian nd)))
					; date standardization
    (list nd ut)))

;; solar-sunrise-sunset --- List of *local* times of sunrise, sunset,
;; and daylight on Gregorian DATE.
;;
;; Corresponding value is #f if there is no sunrise/sunset.
(define (solar-sunrise-sunset date)
  "List of local times of sunrise, sunset, and daylight on Gregorian DATE"
  (let* (; first, get the exact moment of local noon.
         (exact-local-noon (solar-exact-local-noon date))
					; get the the time from the 2000 epoch.
         (t0 (solar-julian-ut-centuries (car exact-local-noon)))
					; store the sidereal time at Greenwich at midnight of UT time.
					; find if summer or winter slightly above the equator
         (equator-rise-set
          (begin (set! solar-sidereal-time-greenwich-midnight 
                       (solar-sidereal-time t0))
                 (solar-sunrise-and-sunset 
                  (list t0 (car (cdr exact-local-noon)))
                  10.0
                  calendar-longitude)))
					; store the spring/summer information,
					; compute sunrise and sunset (two first components of rise-set).
					; length of day is the third component (it is only the difference
					; between sunset and sunrise when there is a sunset and a sunrise)
         (rise-set
          (begin
            (set! solar-spring-or-summer-season 
                  (if (> (car (cdr (cdr equator-rise-set))) 12) 1 0))
            (solar-sunrise-and-sunset 
             (list t0 (car (cdr exact-local-noon)))
             calendar-latitude
             calendar-longitude)))
         (rise (car rise-set))
         (adj-rise (if rise (dst-adjust-time date rise) ))
         (set (car (cdr rise-set)))
         (adj-set (if set (dst-adjust-time date set) ))
         (length  (car (cdr (cdr rise-set)))) )
    (list
     (and rise (calendar-date-equal date (car adj-rise)) (cdr adj-rise))
     (and set (calendar-date-equal date (car adj-set)) (cdr adj-set))
     (solar-daylight length))))


;; Printable form for time expressed in hours.
;; FIXME Format is not working correctly.
(define (solar-daylight time)
  (format #f "~d:~02d"
          (inexact->exact (floor time))
          (inexact->exact (floor (* 60 (- time (floor time)))))))


;; solar-julian-ut-centuries --- Number of Julian centuries elapsed
;; since 1 Jan, 2000 at noon U.T. for Gregorian DATE.
(define (solar-julian-ut-centuries date)
  "Number of Julian centuries elapsed since 1 Jan, 2000 at noon U.T. for Gregorian DATE"
  (/ (- (calendar-absolute-from-gregorian date) 
        (+ 0.5 (calendar-absolute-from-gregorian '(1 1 2000))))
     36525.0))

;; solar-ephemeris-time --- Ephemeris Time at moment TIME.
;;
;; TIME is a pair with the first component being the number of Julian
;; centuries elapsed at 0 Universal Time, and the second component
;; being the universal time.  For instance, the pair corresponding to
;; November 28, 1995 at 16 UT is \(-0.040945 16), -0.040945 being the
;; number of julian centuries elapsed between Jan 1, 2000 at 12 UT and
;; November 28, 1995 at 0 UT.
;;
;; Result is in julian centuries of ephemeris time.
(define (solar-ephemeris-time time)
  "Ephemeris time at moment TIME"
  (let* ((t0 (car time))
         (ut (car (cdr time)))
         (t1 (+ t0 (/ (/ ut 24.0) 36525)))
         (y (+ 2000 (* 100 t1)))
         (dt (* 86400 (solar-ephemeris-correction (inexact->exact (floor y))))))
    (+ t1 (/ (/ dt 86400) 36525))))

;; solar-date-next-longitude --- First moment on or after Julian day number D when sun's longitude is a
;; multiple of L degrees at calendar-location-name with that location's local time (including any daylight savings rules).
;;
;; L must be an integer divisor of 360.
;;
;; Result is in local time expressed astronomical (Julian) day numbers.
;;
;; The values of calendar-daylight-savings-starts,
;; calendar-daylight-savings-starts-time,
;; calendar-daylight-savings-ends,
;; calendar-daylight-savings-ends-time, calendar-daylight-time-offset,
;; and calendar-time-zone are used to interpret local time.
(define (solar-date-next-longitude d l)
  "First moment on or after Julian day number D when the sun's longitude is a multiple of L degrees local time"
  (let* ((long)
         (start d)
         (start-long (solar-longitude d))
         (next (modulo (* l (1+ (inexact->exact (floor (/ start-long l))))) 360))
         (end (+ d (* (/ l 360.0) 400)))
         (end-long (solar-longitude end)))
    (while                 ;; bisection search for nearest minute
     (< 0.00001 (- end start))
     ;; start   <= d    < end
     ;; start-long <= next < end-long when next != 0
     ;; when next = 0, we look for the discontinuity (start-long is near 360
     ;;                and end-long is small (less than l).
     (set! d (/ (+ start end) 2.0))
     (set! long (solar-longitude d))
     (if (or (and (/= next 0) (< long next))
	     (and (= next 0) (< l long)))
	 (begin
	   (set! start d)
	   (set! start-long long))
	 (set! end d)
	 (set! end-long long)))
    (/ (+ start end) 2.0)))

;; solar-horizontal-coordinates --- Azimuth and height of the sun at
;; TIME, LATITUDE, and LONGITUDE.
;;
;; TIME is a pair with the first component being the number of Julian
;; centuries elapsed at 0 Universal Time, and the second component
;; being the universal time.  For instance, the pair corresponding to
;; November 28, 1995 at 16 UT is \(-0.040945 16), -0.040945 being the
;; number of julian centuries elapsed between Jan 1, 2000 at 12 UT and
;; November 28, 1995 at 0 UT.
;;
;; The azimuth is given in degrees as well as the height (between -180
;; and 180).
(define (solar-horizontal-coordinates 
	 time latitude longitude for-sunrise-sunset)
  "Azimuth and height of the sun at TIME, LATITUDE, and LONGITUDE"
  (let* ((ut (car (cdr time)))
         (ec (solar-equatorial-coordinates time for-sunrise-sunset))
         (st (+ solar-sidereal-time-greenwich-midnight
                (* ut 1.00273790935)))
         (ah (- (* st 15) (* 15 (car ec)) (* -1 calendar-longitude)))
					; hour angle (in degrees)
         (de (car (cdr ec)))
         (azimuth (solar-atn2 (- (* (solar-cosine-degrees ah)
				    (solar-sin-degrees latitude))
				 (* (solar-tangent-degrees de)
				    (solar-cosine-degrees latitude)))
                              (solar-sin-degrees ah)))
         (height (solar-arcsin 
                  (+ (* (solar-sin-degrees latitude) (solar-sin-degrees de))
                     (* (solar-cosine-degrees latitude)
                        (solar-cosine-degrees de)
                        (solar-cosine-degrees ah))))))
    (if (> height 180) (set! height (- height 360)))
    (list azimuth height)))

;; solar-equatorial-coordinates --- Right ascension (in hours) and
;; declination (in degrees) of the sun at TIME.
;;
;; TIME is a pair with the first component being the number of Julian
;; centuries elapsed at 0 Universal Time, and the second component
;; being the universal time.  For instance, the pair corresponding to
;; November 28, 1995 at 16 UT is \(-0.040945 16), -0.040945 being the
;; number of julian centuries elapsed between Jan 1, 2000 at 12 UT and
;; November 28, 1995 at 0 UT.
(define (solar-equatorial-coordinates time for-sunrise-sunset)
  "Right ascension (in hours) and declination (in degrees) of the sun at TIME"
  (let* ((tm (solar-ephemeris-time time)) 
	 (ec (solar-ecliptic-coordinates tm for-sunrise-sunset)))
    (list (solar-right-ascension (car ec) (car (cdr ec)))
	  (solar-declination (car ec) (car (cdr ec))))))

;; solar-ecliptic-coordinates --- Apparent longitude of the sun,
;; ecliptic inclination, (both in degrees) equation of time (in hours)
;; and nutation in longitude (in seconds) at moment `time', expressed
;; in julian centuries of Ephemeris Time since January 1st, 2000, at
;; 12 ET.
(define (solar-ecliptic-coordinates time for-sunrise-sunset)
  "Apparent longitude of the sun, ecliptic inclination, equation of time, and nutation in longitude at moment TIME"
  (let* ((l (+ 280.46645
               (* 36000.76983 time)
               (* 0.0003032 time time))) ; sun mean longitude 
         (ml (+ 218.3165
                (* 481267.8813 time))) ; moon mean longitude 
         (m (+ 357.52910
               (* 35999.05030 time)
               (* -0.0001559 time time)
               (* -0.00000048 time time time))) ; sun mean anomaly 
         (i (+ 23.43929111 (* -0.013004167 time)
               (* -0.00000016389 time time)
               (* 0.0000005036 time time time))); mean inclination 
         (c (+ (* (+ 1.914600
                     (* -0.004817 time)
                     (* -0.000014 time time))
                  (solar-sin-degrees m))
               (* (+ 0.019993 (* -0.000101 time))
                  (solar-sin-degrees (* 2 m)))
               (* 0.000290
                  (solar-sin-degrees (* 3 m))))) ; center equation 
         (L (+ l c)) ; total longitude 
         (omega (+ 125.04
                   (* -1934.136 time))) ; longitude of moon's ascending node
                                        ; on the ecliptic
         (nut (if (not for-sunrise-sunset)
		  (+ (* -17.20 (solar-sin-degrees omega))
		     (* -1.32 (solar-sin-degrees (* 2 l)))
		     (* -0.23 (solar-sin-degrees (* 2 ml)))
		     (* 0.21 (solar-sin-degrees (* 2 omega))))
		  #f))
					; nut = nutation in longitude, measured in seconds of angle.
         (ecc (if (not for-sunrise-sunset)
		  (+ 0.016708617
		     (* -0.000042037 time)
		     (* -0.0000001236 time time)) ; eccentricity of earth's orbit
		  #f))
         (app (+ L
                 -0.00569
                 (* -0.00478
                    (solar-sin-degrees omega)))) ; apparent longitude of sun
         (y (if (not for-sunrise-sunset)
		(* (solar-tangent-degrees (/ i 2)) 
		   (solar-tangent-degrees (/ i 2)))
                #f))
         (time-eq (if (not for-sunrise-sunset)
		      (/ (* 12 (+ (* y (solar-sin-degrees (* 2 l)))
				  (* -2 ecc (solar-sin-degrees m))
				  (* 4 ecc y (solar-sin-degrees m) 
				     (solar-cosine-degrees (* 2 l)))
				  (* -0.5 y y  (solar-sin-degrees (* 4 l)))
				  (* -1.25 ecc ecc (solar-sin-degrees (* 2 m)))))
			 3.1415926535)
		      #f)))
					; equation of time, in hours
    (list app i time-eq nut)))


;; solar-longitude --- Longitude of sun on astronomical (Julian) day
;; number D.  Accurary is about 0.0006 degree (about
;; 365.25*24*60*0.0006/360 = 1 minutes).
;;
;; The values of calendar-daylight-savings-starts,
;; calendar-daylight-savings-starts-time,
;; calendar-daylight-savings-ends,
;; calendar-daylight-savings-ends-time, calendar-daylight-time-offset,
;; and calendar-time-zone are used to interpret local time.
(define (solar-longitude d)
  "Longitude of sun on astronomical (Julian) day number D"
  (let* ((a-d (calendar-absolute-from-astro d))
         ;; get Universal Time
         (date (calendar-astro-from-absolute
                (- a-d
                   (if (dst-in-effect a-d)
                       (/ calendar-daylight-time-offset 24.0 60.0) 0)
                   (/ calendar-time-zone 60.0 24.0))))
         ;; get Ephemeris Time
         (date (+ date (solar-ephemeris-correction
                        (extract-calendar-year
                         (calendar-gregorian-from-absolute
                          (inexact->exact (floor
					   (calendar-absolute-from-astro
					    date))))))))
         (U (quotient (- date 2451545) 3652500))
         (longitude
          (+ 4.9353929
             (* 62833.1961680 U)
             (* 0.0000001
                (apply '+
                       (mapcar '(lambda (x)
                                  (* (car x)
                                     (sin (mod
                                           (+ (car (cdr x))
                                              (* (car (cdr (cdr x))) U))
                                           (* 2 pi)))))
                               solar-data-list)))))
         (aberration
          (* 0.0000001 (- (* 17 (cos (+ 3.10 (* 62830.14 U)))) 973)))
         (A1 (mod (+ 2.18 (* U (+ -3375.70 (* 0.36 U)))) (* 2 pi)))
         (A2 (mod (+ 3.51 (* U (+ 125666.39 (* 0.10 U)))) (* 2 pi)))
         (nutation (* -0.0000001 (+ (* 834 (sin A1)) (* 64 (sin A2))))))
    (mod (radians-to-degrees (+ longitude aberration nutation)) 360.0)))

;; solar-data-list --- Taken verbatim.  No comment.
(define solar-data-list
  '((403406 4.721964 1.621043)
    (195207 5.937458 62830.348067)
    (119433 1.115589 62830.821524)
    (112392 5.781616 62829.634302)
    (3891 5.5474 125660.5691)
    (2819 1.5120 125660.984)
    (1721 4.1897 62832.4766)
    (0 1.163 0.813)
    (660 5.415 125659.31)
    (350 4.315 57533.85)
    (334 4.553 -33.931)
    (314 5.198 777137.715)
    (268 5.989 78604.191)
    (242 2.911 5.412)
    (234 1.423 39302.098)
    (158 0.061 -34.861)
    (132 2.317 115067.698)
    (129 3.193 15774.337)
    (114 2.828 5296.670)
    (99 0.52 58849.27)
    (93 4.65 5296.11)
    (86 4.35 -3980.70)
    (78 2.75 52237.69)
    (72 4.50 55076.47)
    (68 3.23 261.08)
    (64 1.22 15773.85)
    (46 0.14 188491.03)
    (38 3.44 -7756.55)
    (37 4.37 264.89)
    (32 1.14 117906.27)
    (29 2.84 55075.75)
    (28 5.96 -7961.39)
    (27 5.09 188489.81)
    (27 1.72 2132.19)
    (25 2.56 109771.03)
    (24 1.92 54868.56)
    (21 0.09 25443.93)
    (21 5.98 -55731.43)
    (20 4.03 60697.74)
    (18 4.47 2132.79)
    (17 0.79 109771.63)
    (14 4.24 -7752.82)
    (13 2.01 188491.91)
    (13 2.65 207.81)
    (13 4.98 29424.63)
    (12 0.93 -7.99)
    (10 2.21 46941.14)
    (10 3.59 -68.29)
    (10 1.50 21463.25)
    (10 2.55 157208.40)))

;; solar-ephemeris-correction --- Ephemeris time minus Universal Time
;; during Gregorian year.  Result is in days.
;;
;;  For the years 1800-1987, the maximum error is 1.9 seconds.  For
;;  the other years, the maximum error is about 30 seconds.
(define (solar-ephemeris-correction year)
  "Ephemeris time minus Universal Time during Gregorian year.  Result is in days."
  (cond ((and (<= 1988 year) (< year 2020))
         (/ (+ year -2000 67.0) 60.0 60.0 24.0))
        ((and (<= 1900 year) (< year 1988))
         (let* ((theta (/ (- (calendar-astro-from-absolute
                              (calendar-absolute-from-gregorian
                               (list 7 1 year)))
                             (calendar-astro-from-absolute
                              (calendar-absolute-from-gregorian
                               '(1 1 1900))))
                          36525.0))
                (theta2 (* theta theta))
                (theta3 (* theta2 theta))
                (theta4 (* theta2 theta2))
                (theta5 (* theta3 theta2)))
           (+ -0.00002
              (* 0.000297 theta)
              (* 0.025184 theta2)
              (* -0.181133 theta3)
              (* 0.553040 theta4)
              (* -0.861938 theta5)
              (* 0.677066 theta3 theta3)
              (* -0.212591 theta4 theta3))))
        ((and (<= 1800 year) (< year 1900))
         (let* ((theta (/ (- (calendar-astro-from-absolute
                              (calendar-absolute-from-gregorian
                               (list 7 1 year)))
                             (calendar-astro-from-absolute
                              (calendar-absolute-from-gregorian
                               '(1 1 1900))))
                          36525.0))
                (theta2 (* theta theta))
                (theta3 (* theta2 theta))
                (theta4 (* theta2 theta2))
                (theta5 (* theta3 theta2)))
           (+ -0.000009
              (* 0.003844 theta)
              (* 0.083563 theta2)
              (* 0.865736 theta3)
              (* 4.867575 theta4)
              (* 15.845535 theta5)
              (* 31.332267 theta3 theta3)
              (* 38.291999 theta4 theta3)
              (* 28.316289 theta4 theta4)
              (* 11.636204 theta4 theta5)
              (* 2.043794 theta5 theta5))))
        ((and (<= 1620 year) (< year 1800))
         (let ((x (/ (- year 1600) 10.0)))
           (/ (+ (* 2.19167 x x) (* -40.675 x) 196.58333) 60.0 60.0 24.0)))
        (#t (let* ((tmp (- (calendar-astro-from-absolute
			    (calendar-absolute-from-gregorian
			     (list 1 1 year)))
			   2382148))
		   (second (- (/ (* tmp tmp) 41048480.0) 15)))
	      (/ second 60.0 60.0 24.0)))))

;; solar-sidereal-time -- Sidereal time (in hours) in Greenwich.
;;
;; At T0=Julian centuries of universal time.  T0 must correspond to 0
;; hours UT.
(define (solar-sidereal-time t0)
  "Sidereal time (in hours) in Greenwich"
  (let* ((mean-sid-time (+ 6.6973746
                           (* 2400.051337 t0)
                           (* 0.0000258622 t0 t0)
                           (* -0.0000000017222 t0 t0 t0)))
	 (et (solar-ephemeris-time (list t0 0.0)))
	 (nut-i (solar-ecliptic-coordinates et #f))
	 (nut (car (cdr (cdr (cdr nut-i))))) ; nutation
	 (i (car (cdr nut-i)))) ; inclination
    (mod (+ (mod (+ mean-sid-time 
                    (/ (/ (* nut (solar-cosine-degrees i)) 15) 3600)) 24.0)
	    24.0)
	 24.0)))

;; solar-time-equation --- Equation of time expressed in hours at
;; Gregorian DATE at Universal time UT.
(define (solar-time-equation date ut)
  "Equation of time expressed in hours in Gregorian DATE at Universal time UT"
  (let* ((et (solar-date-to-et date ut))
         (ec (solar-ecliptic-coordinates et #f)))
    (car (cdr (cdr ec)))))

;; solar-date-to-et --- Ephemeris Time at Gregorian DATE at Universal
;; Time UT (in hours).  Expressed in julian centuries of Ephemeris
;; Time.
(define (solar-date-to-et date ut)
  (let ((t0 (solar-julian-ut-centuries date)))
    (solar-ephemeris-time (list t0 ut))))

;; solar-equinoxes/solstices --- Date of equinox/solstice K for YEAR.
;; K=0, spring equinox; K=1, summer solstice; K=2, fall equinox; K=3,
;; winter solstice.  RESULT is a gregorian local date.
;;
;; Accurate to less than a minute between 1951 and 2050.
(define (solar-equinoxes/solstices k year)
  "Date of equinox/solstice K for YEAR"
  (let* ((JDE0 (solar-mean-equinoxes/solstices k year))
         (T (/ (- JDE0 2451545.0) 36525))
         (W (- (* 35999.373 T) 2.47))
         (Delta-lambda (+ 1 (* 0.0334 (solar-cosine-degrees W))
			  (* 0.0007 (solar-cosine-degrees (* 2 W)))))
         (S (apply '+ (mapcar '(lambda(x) 
                                 (* (car x) (solar-cosine-degrees 
                                             (+ (* (car (cdr (cdr x))) T)
						(car (cdr x)))))) 
                              solar-seasons-data)))
         (JDE (+ JDE0 (/ (* 0.00001 S) Delta-lambda)))
         (correction (+ 102.3 (* 123.5 T) (* 32.5 T T))) 
					; ephemeris time correction
         (JD (- JDE (/ correction 86400)))
         (date (calendar-gregorian-from-absolute (inexact->exact (floor (- JD 1721424.5)))))
         (time (- (- JD 0.5) (inexact->exact (floor (- JD 0.5)))))
         )
    (list (car date) (+ (car (cdr date)) time
			(/ (/ calendar-time-zone 60.0) 24.0))
	  (car (cdr (cdr date))))))

;; solar-mean-equinoxes/solstices --- Julian day of mean
;; equinox/solstice K for YEAR.  K=0, spring equinox; K=1, summer
;; solstice; K=2, fall equinox; K=3, winter solstice.  These formulas
;; are only to be used between 1000 BC and 3000 AD.
; from Meeus, 1991, page 166
(define (solar-mean-equinoxes/solstices k year)
  "Julian day of mean equinox/solstice K for YEAR"
  (let ((y (/ year 1000.0))
        (z (/ (- year 2000) 1000.0)))
    (if (< year 1000) ; actually between -1000 and 1000
	(cond ((equal? k 0) (+ 1721139.29189
			       (*  365242.13740 y)
			       (* 0.06134 y y)
			       (* 0.00111 y y y)
			       (* -0.00071 y y y y)))
	      ((equal? k 1) (+ 1721233.25401
			       (* 365241.72562 y)
			       (* -0.05323 y y)
			       (* 0.00907 y y y)
			       (* 0.00025 y y y y)))
	      ((equal? k 2) (+ 1721325.70455
			       (* 365242.49558 y)
			       (* -0.11677 y y)
			       (* -0.00297 y y y)
			       (* 0.00074 y y y y)))
	      ((equal? k 3) (+ 1721414.39987
			       (* 365242.88257 y)
			       (* -0.00769 y y)
			       (* -0.00933 y y y)
			       (* -0.00006 y y y y))))
					; actually between 1000 and 3000
	(cond ((equal? k 0) (+ 2451623.80984
			       (* 365242.37404  z)
			       (* 0.05169 z z)
			       (* -0.00411 z z z)
			       (* -0.00057 z z z z)))
	      ((equal? k 1) (+ 2451716.56767
			       (* 365241.62603 z)
			       (* 0.00325 z z)
			       (* 0.00888 z z z)
			       (* -0.00030 z z z z)))
	      ((equal? k 2) (+ 2451810.21715
			       (* 365242.01767 z)
			       (* -0.11575 z z)
			       (* 0.00337 z z z)
			       (* 0.00078 z z z z)))
	      ((equal? k 3) (+ 2451900.05952
			       (* 365242.74049 z)
			       (* -0.06223 z z)
			       (* -0.00823 z z z)
			       (* 0.00032 z z z z)))))))

;; solar-seasons-data --- Taken verbatim.  No comment.
; from Meeus, 1991, page 167
(define solar-seasons-data
  '((485 324.96 1934.136)
    (203 337.23 32964.467)
    (199 342.08 20.186)
    (182 27.85 445267.112)
    (156 73.14 45036.886)
    (136 171.52 22518.443)
    (77 222.54 65928.934)
    (74 296.72 3034.906)
    (70 243.58 9037.513)
    (58 119.81 33718.147)
    (52 297.17 150.678)
    (50 21.02 2281.226)
    (45 247.54 29929.562)
    (44 325.15 31555.956)
    (29 60.93 4443.417)
    (18 155.12 67555.328)
    (17 288.79 4562.452)
    (16 198.04 62894.029)
    (14 199.76 31436.921)
    (12 95.39 14577.848)
    (12 287.11 31931.756)
    (12 320.81 34777.259)
    (9 227.73 1222.114)
    (8 15.45 16859.074)))


;;; DST Functions

;; Cache for result of calendar-current-time-zone.
(define calendar-current-time-zone-cache nil)

;; Absolute local date of time X; local time is UTC-DIFF seconds from
;; UTC.  
;;
;; X is (HIGH . LOW) or (HIGH LOW . IGNORED) where HIGH and LOW are
;; the high and low 16 bits, respectively, of the number of seconds
;; since 1970-01-01 00:00:00 UTC, ignoring leap seconds.
;;
;; Returns the pair (ABS-DATE . SECONDS) where SECONDS after local
;; midnight on absolute date ABS-DATE is the equivalent moment to X.
(define (calendar-absolute-from-time x utc-diff)
  "Absolute local date of time X"
  (cons (+ calendar-system-time-basis
	   (quotient x  (* 60 60 24)))
	(modulo x (* 60 60 24))))

;; Time of absolute date ABS-DATE, S seconds after midnight.
;;
;; Returns the number of seconds since the system clock basis,
;; ignoring leap seconds, that is the equivalent moment to S seconds
;; after midnight UTC on absolute date ABS-DATE.
(define (calendar-time-from-absolute abs-date s)
  "Time of absolute date ABS-DATE, S seconds after midnight"
  (+ s (* 60 60 24 (- abs-date calendar-system-time-basis))))

;; Return the time of the next time zone transition after TIME.  Both
;; TIME and the result are acceptable arguments to current-time-zone.
;; Return #f if no such transition can be found.
(define (calendar-next-time-zone-transition time)
  "The time of the next time zone transition after TIME"
  (let* ((quarter-multiple (* 91 24 3600));; approx = (seconds per quarter year)
	 (time-zone (current-time-zone time))
	 (time-utc-diff (car time-zone))
         (hi #f)
	 (hi-zone #f)
         (hi-utc-diff time-utc-diff)
         (quarters '(2 1 3)))
    ;; Heuristic: probe the time zone offset in the next three calendar
    ;; quarters, looking for a time zone offset different from TIME.
    (while (and (not (null? quarters)) (eq? time-utc-diff hi-utc-diff))
	   (set! hi (+ time (* (car quarters) quarter-multiple))) ;; was (car time) rather than time DPV
	   (set! hi-zone (current-time-zone hi))
	   (set! hi-utc-diff (car hi-zone))
	   (set! quarters (cdr quarters)))
    (and
     time-utc-diff
     hi-utc-diff
     (not (eq? time-utc-diff hi-utc-diff))
     ;; Now HI is after the next time zone transition.
     ;; Set LO to TIME, and then binary search to increase LO and decrease HI
     ;; until LO is just before and HI is just after the time zone transition.
     (let ((probe #f)
	   (lo time))
       (while
	;; Set PROBE to halfway between LO and HI, rounding down.
	;; If PROBE equals LO, we are done.
	(begin
	  (set! probe (quotient (+ lo hi) 2))
	  (not (equal? lo probe)))
	(if (eq? (car (current-time-zone probe)) hi-utc-diff)
	    (set! hi probe)
	    (set! lo probe)))
	hi))))

;; Return daylight transition rule for ABS-DATE, UTC-DIFF sec offset
;; from UTC.  ABS-DATE must specify a day that contains a daylight
;; savings transition.  The result has the proper form for
;; calendar-daylight-savings-starts'.
(define (calendar-time-zone-daylight-rules abs-date utc-diff)
  "Return daylight transition rule for ABS-DATE, UTC-DIFF sec offset from UTC"
  (let* ((date (calendar-gregorian-from-absolute abs-date))
	 (weekday (modulo abs-date 7))
	 (m (extract-calendar-month date))
	 (d (extract-calendar-day date))
	 (y (extract-calendar-year date))
         (last (calendar-last-day-of-month m y))
	 (candidate-rules
	  (append
	   ;; Day D of month M.
	   (list (list 'list m d 'year))
	   ;; The first WEEKDAY of month M.
           (if (< d 8)
               (list (list 'calendar-nth-named-day 1 weekday m 'year)) '())
	   ;; The last WEEKDAY of month M.
           (if (> d (- last 7))
               (list (list 'calendar-nth-named-day -1 weekday m 'year)) '())
	   ;; The first WEEKDAY after day J of month M, for D-6 < J <= D.
           (let ((l nil))
	     (let ((j (max 2 (- d 6))))
	       (while (<= j (min d (- last 8)))
		      (set! l (cons (list 'calendar-nth-named-day 1 weekday m 'year j) l))
		      (set! j (1+ j))))
	     l)
	   ;; 01-01 and 07-01 for this year's Persian calendar.
	   (if (and (= m 3) (<= 20 d) (<= d 21))
	       '((calendar-gregorian-from-absolute
		  (calendar-absolute-from-persian
		   (list 1 1 (- year 621))))) '())
	   (if (and (= m 9) (<= 22 d) (<= d 23))
	       '((calendar-gregorian-from-absolute
		  (calendar-absolute-from-persian
		   (list 7 1 (- year 621))))) '())))
	 (prevday-sec (- -1 utc-diff)) ;; last sec of previous local day
	 (year (1+ y)))
    ;; Scan through the next few years until only one rule remains.
    (while
     (let ((rules candidate-rules)
	   (new-rules nil))
       (while
	(let*
	    ((rule (car rules))
	     (date
	      ;; The following is much faster than
	      ;; (calendar-absolute-from-gregorian (eval rule)).
	      (cond ((eq? (car rule) 'calendar-nth-named-day)
		     (local-eval (cons 'calendar-nth-named-absday (cdr rule)) (the-environment)))
		    ((eq? (car rule) 'calendar-gregorian-from-absolute)
		     (local-eval (car (cdr rule)) (the-environment)))
		    (#t (let ((g (local-eval rule (the-environment))))
			  (calendar-absolute-from-gregorian g))))))
	  (or (equal?
	       (current-time-zone
		(calendar-time-from-absolute date prevday-sec))
	       (current-time-zone
		(calendar-time-from-absolute (1+ date) prevday-sec)))
	      (set! new-rules (cons rule new-rules)))
	  (set! rules (cdr rules))
	  (not (null? rules)))
	#t)
       ;; If no rules remain, just use the first candidate rule;
       ;; it's wrong in general, but it's right for at least one year.
       (set! candidate-rules (if (not (null? new-rules)) 
				 (reverse! new-rules)
				 (list (car candidate-rules))))
       (set! year (1+ year))
       (not (null? (cdr candidate-rules))))
     #t)
    (car candidate-rules)))

;; Return UTC difference, dst offset, names and rules for current time
;; zone.
;;
;; Returns (UTC-DIFF DST-OFFSET STD-ZONE DST-ZONE DST-STARTS DST-ENDS
;; DST-STARTS-TIME DST-ENDS-TIME), based on a heuristic probing of
;; what the system knows:
;;
;; UTC-DIFF is an integer specifying the number of minutes difference
;;   between standard time in the current time zone and Coordinated
;;   Universal Time (Greenwich Mean Time).  A negative value means west
;;   of Greenwich.
;; DST-OFFSET is an integer giving the daylight savings time offset in minutes.
;; STD-ZONE is a string giving the name of the time zone when no seasonal time
;;    adjustment is in effect.
;; DST-ZONE is a string giving the name of the time zone when there is a seasonal
;;    time adjustment in effect.
;; DST-STARTS and DST-ENDS are sexps in the variable `year' giving the daylight
;;    savings time start and end rules, in the form expected by
;;    `calendar-daylight-savings-starts'.
;; DST-STARTS-TIME and DST-ENDS-TIME are integers giving the number of minutes
;;    after midnight that daylight savings time starts and ends.
;;
;; Some operating systems cannot provide all this information; in this
;; case, `calendar-current-time-zone' returns a list containing #f
;; for the data it can't find.
;;
;; I removed the division by 60 on the time zone differences in the
;; first two elements of the return list because when they are used,
;; they are assumed to be minutes.
(define (calendar-current-time-zone)
  "Return UTC difference, dst offset, names and rules for current time zone"
  (or
   (not (null? calendar-current-time-zone-cache))
   (set!
    calendar-current-time-zone-cache
    (let* ((t0 (current-time))
	   (t0-zone (current-time-zone t0))
	   (t0-utc-diff (car t0-zone))
	   (t0-name (car (cdr t0-zone))))
      (if (null? t0-utc-diff)
	  ;; Little or no time zone information is available.
	  (list #f #f t0-name t0-name #f #f #f #f)
	  (let* ((t1 (calendar-next-time-zone-transition t0))
		 (t2 (and t1 (calendar-next-time-zone-transition t1))))
	    (if (not t2)
		;; This locale does not have daylight savings time.
		(list t0-utc-diff 0 t0-name t0-name #f #f 0 0)
		;; Use heuristics to find daylight savings parameters.
		(let* ((t1-zone (current-time-zone t1))
		       (t1-utc-diff (car t1-zone))
		       (t1-name (car (cdr t1-zone)))
		       (t1-date-sec (calendar-absolute-from-time t1 t0-utc-diff))
		       (t2-date-sec (calendar-absolute-from-time t2 t1-utc-diff))
		       (t1-rules (calendar-time-zone-daylight-rules
				  (car t1-date-sec) t0-utc-diff))
		       (t2-rules (calendar-time-zone-daylight-rules
				  (car t2-date-sec) t1-utc-diff))
		       (t1-time (quotient (cdr t1-date-sec) 60))
		       (t2-time (quotient (cdr t2-date-sec) 60)))
		  (cons
		   (min t0-utc-diff t1-utc-diff)
		   (cons
		    (abs (- t0-utc-diff t1-utc-diff))
		    (if (< t0-utc-diff t1-utc-diff)
			(list t0-name t1-name t1-rules t2-rules t1-time t2-time)
			(list t1-name t0-name t2-rules t1-rules t2-time t1-time)
			)))))))))))

;;; The following eight defvars relating to daylight savings time should NOT be
;;; marked to go into loaddefs.el where they would be evaluated when Emacs is
;;; dumped.  These variables' appropriate values depend on the conditions under
;;; which the code is INVOKED; so it's inappropriate to initialize them when
;;; Emacs is dumped---they should be initialized when calendar.el is loaded.
;;; They default to US Eastern time if time zone info is not available.

(calendar-current-time-zone)

;; *Number of minutes difference between local standard time at
;; `calendar-location-name' and Coordinated Universal (Greenwich)
;; Time.  For example, -300 for New York City, -480 for Los Angeles.
(define calendar-time-zone (or (car calendar-current-time-zone-cache) -300))

;; *Number of minutes difference between daylight savings and standard time.
;;  
;; If the locale never uses daylight savings time, set this to 0.
(define calendar-daylight-time-offset
  (or (car (cdr calendar-current-time-zone-cache)) 60))

;; *Abbreviated name of standard time zone at
;; `calendar-location-name'.  For example, \"EST\" in New York City,
;; \"PST\" for Los Angeles.
(define calendar-standard-time-zone-name
  (or (car (list-cdr-ref calendar-current-time-zone-cache 2)) "EST"))

;; *Abbreviated name of daylight-savings time zone at
;; `calendar-location-name'.  For example, \"EDT\" in New York City,
;; \"PDT\" for Los Angeles.
(define calendar-daylight-time-zone-name
  (or (car (list-cdr-ref calendar-current-time-zone-cache 3)) "EDT"))

;; *Sexp giving the date on which daylight savings time starts.  This
;; is an expression in the variable `year' whose value gives the
;; Gregorian date in the form (month day year) on which daylight
;; savings time starts.  It is used to determine the starting date of
;; daylight savings time for the holiday list and for correcting times
;; of day in the solar and lunar calculations.
;;
;; For example, if daylight savings time is mandated to start on
;; October 1, you would set `calendar-daylight-savings-starts' to
;;
;;       '(10 1 year)
;;
;; If it starts on the first Sunday in April, you would set it to
;;
;;       '(calendar-nth-named-day 1 0 4 year)
;;
;; If the locale never uses daylight savings time, set this to #f.
(define calendar-daylight-savings-starts
  (or (car (list-cdr-ref calendar-current-time-zone-cache 4))
      (and (not (= calendar-daylight-time-offset 0))
	   '(calendar-nth-named-day 1 0 4 year))))



;; *Sexp giving the date on which daylight savings time ends.  This is
;; an expression in the variable `year' whose value gives the
;; Gregorian date in the form (month day year) on which daylight
;; savings time ends.  It is used to determine the starting date of
;; daylight savings time for the holiday list and for correcting times
;; of day in the solar and lunar calculations.
;;
;; For example, if daylight savings time ends on the last Sunday in
;; October:
;;
;;       '(calendar-nth-named-day -1 0 10 year)
;;
;; If the locale never uses daylight savings time, set this to #f.
(define calendar-daylight-savings-ends
  (or (car (list-cdr-ref calendar-current-time-zone-cache 5))
      (and (not (= calendar-daylight-time-offset 0))
	   '(calendar-nth-named-day -1 0 10 year))))

;; *Number of minutes after midnight that daylight savings time starts.  
(define calendar-daylight-savings-starts-time
  (or (car (list-cdr-ref calendar-current-time-zone-cache 6)) 120))

;; *Number of minutes after midnight that daylight savings time ends.  
(define calendar-daylight-savings-ends-time
  (or (car (list-cdr-ref calendar-current-time-zone-cache 7))
      calendar-daylight-savings-starts-time))

;; True if on absolute DATE daylight savings time is in effect.
;; Fractional part of DATE is local standard time of day.
(define (dst-in-effect date)
  "#t if daylight savings time is in effect on DATE"
  (let* ((year (extract-calendar-year
                (calendar-gregorian-from-absolute (inexact->exact (floor date)))))
         (dst-starts-gregorian (local-eval calendar-daylight-savings-starts (the-environment)))
         (dst-ends-gregorian (local-eval calendar-daylight-savings-ends (the-environment)))
         (dst-starts (and dst-starts-gregorian
                          (+ (calendar-absolute-from-gregorian
                              dst-starts-gregorian)
                             (/ calendar-daylight-savings-starts-time
                                60.0 24.0))))
         (dst-ends (and dst-ends-gregorian
                        (+ (calendar-absolute-from-gregorian
                            dst-ends-gregorian)
                           (/ (- calendar-daylight-savings-ends-time
                                 calendar-daylight-time-offset)
                              60.0 24.0)))))
    (and dst-starts dst-ends
         (if (< dst-starts dst-ends)
             (and (<= dst-starts date) (< date dst-ends))
	     (or (<= dst-starts date) (< date dst-ends))))))


;; Adjust, to account for dst on DATE, decimal fraction standard TIME.
;; Returns a list (date adj-time zone) where `date' and `adj-time' are
;; the values adjusted for `zone'; here `date' is a list (month day
;; year), `adj-time' is a decimal fraction time, and `zone' is a
;; string.
;;
;; Optional parameter STYLE forces the result time to be standard time
;; when its value is 'standard and daylight savings time (if
;; available) when its value is 'daylight.
;;
;; Conversion to daylight savings time is done according to
;; `calendar-daylight-savings-starts',
;; `calendar-daylight-savings-ends',
;; `calendar-daylight-savings-starts-time',
;; `calendar-daylight-savings-ends-time', and
;; `calendar-daylight-savings-offset'.
(define (dst-adjust-time date time . style)
  "Adjust TIME to dst status on DATE"
  (let* ((rounded-abs-date (+ (calendar-absolute-from-gregorian date)
			      (/ (round (* 60 time)) 60.0 24.0)))
         (dst (dst-in-effect rounded-abs-date))
	 (time-zone (if dst
			calendar-daylight-time-zone-name
			calendar-standard-time-zone-name))
	 (time (+ rounded-abs-date
                  (if dst (/ calendar-daylight-time-offset 24.0 60.0) 0))))
    (list (calendar-gregorian-from-absolute (inexact->exact (truncate time)))
          (* 24.0 (- time (truncate time)))
          time-zone)))


;;; French Revolutionary Calendar

;; Absolute date of start of French Revolutionary calendar = September
;; 22, 1792.
(define french-calendar-epoch (calendar-absolute-from-gregorian '(9 22 1792)))

;; True if YEAR is a leap year on the French Revolutionary calendar.
;; For Gregorian years 1793 to 1805, the years of actual operation of
;; the calendar, follows historical practice based on equinoxes (years
;; 3, 7, and 11 were leap years; 15 and 20 would have been leap
;; years).  For later years uses the proposed rule of Romme (never
;; adopted)--leap years fall every four years except century years not
;; divisible 400 and century years that are multiples of 4000.
(define (french-calendar-leap-year? year)
  (or (memq year '(3 7 11));; Actual practice--based on equinoxes
      (memq year '(15 20)) ;; Anticipated practice--based on equinoxes
      (and (> year 20)     ;; Romme's proposal--never adopted
           (zero? (modulo year 4))
           (not (memq (modulo year 400) '(100 200 300)))
           (not (zero? (modulo year 4000))))))

;; Return last day of MONTH, YEAR on the French Revolutionary
;; calendar.  The 13th month is not really a month, but the 5 (6 in
;; leap years) day period of `sansculottides' at the end of the year.
(define (french-calendar-last-day-of-month month year)
  (if (< month 13)
      30
      (if (french-calendar-leap-year? year)
	  6
	  5)))

;; Compute absolute date from French Revolutionary date DATE.  The
;; absolute date is the number of days elapsed since the (imaginary)
;; Gregorian date Sunday, December 31, 1 BC.
(define (calendar-absolute-from-french date)
  (let ((month (extract-calendar-month date))
        (day (extract-calendar-day date))
        (year (extract-calendar-year date)))
    (+ (* 365 (1- year));; Days in prior years
       ;; Leap days in prior years
       (if (< year 20)
           (quotient year 4);; Actual and anticipated practice (years 3, 7, 11, 15)
         ;; Romme's proposed rule (using the Principle of Inclusion/Exclusion)
         (+ (quotient (1- year) 4);; Luckily, there were 4 leap years before year 20
            (- (quotient (1- year) 100))
            (quotient (1- year) 400)
            (- (quotient (1- year) 4000))))
       (* 30 (1- month));; Days in prior months this year
       day;; Days so far this month
       (1- french-calendar-epoch))));; Days before start of calendar

;; Compute the French Revolutionary equivalent for absolute date DATE.
;; The result is a list of the form (MONTH DAY YEAR).  The absolute
;; date is the number of days elapsed since the \(imaginary) Gregorian
;; date Sunday, December 31, 1 BC.
(define (calendar-french-from-absolute date)
  (if (< date french-calendar-epoch)
      (list 0 0 0);; pre-French Revolutionary date
      (let* ((approx              ;; Approximation from below.
	      (quotient (- date french-calendar-epoch) 366))
	     (year                ;; Search forward from the approximation.
	      (+ approx
		 (let ((y approx)
		       (sum 0))
		   (while (>= date (calendar-absolute-from-french (list 1 1 (1+ y))))
			  (set! sum (+ sum 1))
			  (set! y (1+ y)))
		   sum)))
	     (month               ;; Search forward from Vendemiaire.
	      (1+ (let ((m 1)
			(sum 0))
		    (while (> date
			      (calendar-absolute-from-french
			       (list m (french-calendar-last-day-of-month m year) year)))
			   (set! sum (+ sum 1))
			   (set! m (1+ m)))
		    sum)))
	     (day                   ;; Calculate the day by subtraction.
	      (- date
		 (1- (calendar-absolute-from-french (list month 1 year))))))
	(list month day year))))


;;; Coptic and Ethiopic Calendar

;; Absolute date of start of Coptic calendar = August 29, 284
;; A.D. (Julian).
(define coptic-calendar-epoch (calendar-absolute-from-julian '(8 29 284)))

(define coptic-name "Coptic")

;; True if YEAR is a leap year on the Coptic calendar.
(define (coptic-calendar-leap-year? year)
  (zero? (modulo (1+ year) 4)))

;; Return last day of MONTH, YEAR on the Coptic calendar.  The 13th
;; month is not really a month, but the 5 (6 in leap years) day period
;; of Nisi (Kebus) at the end of the year.
(define (coptic-calendar-last-day-of-month month year)
  (if (< month 13)
      30
      (if (coptic-calendar-leap-year? year)
	  6
	  5)))

;; Compute absolute date from Coptic date DATE.  The absolute date is
;; the number of days elapsed since the (imaginary) Gregorian date
;; Sunday, December 31, 1 BC.
(define (calendar-absolute-from-coptic date)
  (let ((month (extract-calendar-month date))
        (day (extract-calendar-day date))
        (year (extract-calendar-year date)))
    (+ (1- coptic-calendar-epoch);; Days before start of calendar
       (* 365 (1- year))         ;; Days in prior years
       (quotient year 4)         ;; Leap days in prior years
       (* 30 (1- month))         ;; Days in prior months this year
       day)))                    ;; Days so far this month
       

;; Compute the Coptic equivalent for absolute date DATE.  The result
;; is a list of the form (MONTH DAY YEAR).  The absolute date is the
;; number of days elapsed since the imaginary Gregorian date Sunday,
;; December 31, 1 BC.
(define (calendar-coptic-from-absolute date)
  (if (< date coptic-calendar-epoch)
      (list 0 0 0);; pre-Coptic date
      (let* ((approx (quotient (- date coptic-calendar-epoch)
			       366))   ;; Approximation from below.
	     (year              ;; Search forward from the approximation.
	      (+ approx
		 (let ((y approx)
		       (sum 0))
		   (while (>= date (calendar-absolute-from-coptic (list 1 1 (1+ y))))
			  (set! sum (+ sum 1))
			  (set! y (1+ y)))
		   sum)))
	     (month             ;; Search forward from Tot.
	      (1+ (let ((m 1)
			(sum 0))
		    (while (> date
			      (calendar-absolute-from-coptic
			       (list m (coptic-calendar-last-day-of-month m year) year)))
			   (set! sum (+ sum 1))
			   (set! m (1+ m)))
		    sum)))
	     (day                ;; Calculate the day by subtraction.
	      (- date
		 (1- (calendar-absolute-from-coptic (list month 1 year))))))
	(list month day year))))

;; Absolute date of start of Ethiopic calendar = August 29, 8
;; C.E. (Julian).
(define ethiopic-calendar-epoch 2796)

(define ethiopic-name "Ethiopic")

;; Compute absolute date from Ethiopic date DATE.  The absolute date
;; is the number of days elapsed since the (imaginary) Gregorian date
;; Sunday, December 31, 1 BC.
(define (calendar-absolute-from-ethiopic date)
  (- (+ ethiopic-calendar-epoch 
	(calendar-absolute-from-coptic date)) 
     coptic-calendar-epoch))

;; Compute the Ethiopic equivalent for absolute date DATE.  The result
;; is a list of the form (MONTH DAY YEAR).  The absolute date is the
;; number of days elapsed since the imaginary Gregorian date Sunday,
;; December 31, 1 BC.
(define (calendar-ethiopic-from-absolute date)
  (if (< date ethiopic-calendar-epoch)
      (list 0 0 0)
      (calendar-coptic-from-absolute (- (+ coptic-calendar-epoch 
					   date) 
					ethiopic-calendar-epoch))))


;;; ISO Calendar

;; The number of days elapsed between the Gregorian date 12/31/1 BC
;; and DATE.  The `ISO year' corresponds approximately to the
;; Gregorian year, but weeks start on Monday and end on Sunday.  The
;; first week of the ISO year is the first such week in which at least
;; 4 days are in a year.  The ISO commercial DATE has the form (week
;; day year) in which week is in the range 1..52 and day is in the
;; range 0..6 (1 = Monday, 2 = Tuesday, ..., 0 = Sunday).  The
;; Gregorian date Sunday, December 31, 1 BC is imaginary.
(define (calendar-absolute-from-iso date)
  (let* ((week (extract-calendar-month date))
         (day (extract-calendar-day date))
         (year (extract-calendar-year date)))
    (+ (calendar-dayname-on-or-before
        1 (+ 3 (calendar-absolute-from-gregorian (list 1 1 year))))
       (* 7 (1- week))
       (if (= day 0) 6 (1- day)))))

;; Compute the `ISO commercial date' corresponding to the absolute
;; DATE.  The ISO year corresponds approximately to the Gregorian
;; year, but weeks start on Monday and end on Sunday.  The first week
;; of the ISO year is the first such week in which at least 4 days are
;; in a year.  The ISO commercial date has the form (week day year) in
;; which week is in the range 1..52 and day is in the range 0..6 (1 =
;; Monday, 2 = Tuesday, ..., 0 = Sunday).  The absolute date is the
;; number of days elapsed since the (imaginary) Gregorian date Sunday,
;; December 31, 1 BC.
(define (calendar-iso-from-absolute date)
  (let* ((approx (extract-calendar-year
                  (calendar-gregorian-from-absolute (- date 3))))
         (year (+ approx
		  (let ((y approx)
			(sum 0))
		    (while (>= date (calendar-absolute-from-iso (list 1 1 (1+ y))))
			   (set! sum (+ sum 1))
			   (set! y (1+ y)))
		    sum))))
    (list
     (1+ (quotient (- date (calendar-absolute-from-iso (list 1 1 year))) 7))
     (modulo date 7)
     year)))


;;; Persian Calendar

;; Absolute date of start of Persian calendar = March 19, 622
;; A.D. (Julian).
(define persian-calendar-epoch (calendar-absolute-from-julian '(3 19 622)))

;; True if YEAR is a leap year on the Persian calendar.
(define (persian-calendar-leap-year? year)
  (< (mod (* (mod (mod (if (<= 0 year)
					; No year zero
                           (+ year 2346)
			   (+ year 2347))
                       2820)
                  768)
	     683)
	  2820)
     683))

;; Return last day of MONTH, YEAR on the Persian calendar.
(define (persian-calendar-last-day-of-month month year)
  (cond
   ((< month 7) 31)
   ((or (< month 12) (persian-calendar-leap-year? year)) 30)
   (#t 29)))

;; Compute absolute date from Persian date DATE.  The absolute date is
;; the number of days elapsed since the (imaginary) Gregorian date
;; Sunday, December 31, 1 BC.
(define (calendar-absolute-from-persian date)
  (let ((month (extract-calendar-month date))
        (day (extract-calendar-day date))
        (year (extract-calendar-year date)))
    (if (< year 0)
        (+ (calendar-absolute-from-persian
            (list month day (1+ (mod year 2820))))
           (* 1029983 (quotient (- year (mod year 2820)) 2820)))
	(+ (1- persian-calendar-epoch); Days before epoch
	   (* 365 (1- year)) ; Days in prior years.
	   (* 683        ; Leap days in prior 2820-year cycles
	      (quotient (+ year 2345) 2820))
	   (* 186        ; Leap days in prior 768 year cycles
	      (quotient (mod (+ year 2345) 2820) 768))
	   (quotient; Leap years in current 768 or 516 year cycle
	    (* 683 (mod (mod (+ year 2345) 2820) 768))
	    2820)
	   -568          ; Leap years in Persian years -2345...-1
	   (let ((m 1)
		 (sum 0))
	     (while (< m month)
		    (set! sum (+ sum (persian-calendar-last-day-of-month m year)))
		    (set! m (1+ m)))
	     sum)         ; Days in prior months this year.
	   day))))        ; Days so far this month.

;; Persian year corresponding to the absolute DATE.
(define (calendar-persian-year-from-absolute date)
  (let* ((d0        ; Prior days since start of 2820 cycles
          (- date (calendar-absolute-from-persian (list 1 1 -2345))))
         (n2820     ; Completed 2820-year cycles
          (quotient d0 1029983))
         (d1        ; Prior days not in n2820
          (mod d0 1029983))
         (n768      ; 768-year cycles not in n2820
          (quotient d1 280506))
         (d2        ; Prior days not in n2820 or n768
          (mod d1 280506))
         (n1        ; Years not in n2820 or n768
	  (quotient (+ (* 2820 d2) (* 2820 366)) 1029983))
         (year (+ (* 2820 n2820); Complete 2820 year cycles
                  (* 768 n768)  ; Complete 768 year cycles
                  (if           ; Remaining years
					; Last day of 2820 year cycle
		   (= d1 1029617)
		   (1- n1)
		   n1)
                  -2345)))      ; Years before year 1
    (if (< year 1)
        (1- year); No year zero
	year)))

;; Compute the Persian equivalent for absolute date DATE.  The result
;; is a list of the form (MONTH DAY YEAR).  The absolute date is the
;; number of days elapsed since the imaginary Gregorian date Sunday,
;; December 31, 1 BC.
(define (calendar-persian-from-absolute date)
  (let* ((year (calendar-persian-year-from-absolute date))
         (month         ; Search forward from Farvardin
          (1+ (let ((m 1)
		    (sum 0))
		(while (> date (calendar-absolute-from-persian
				(list m (persian-calendar-last-day-of-month m year) year)))
		       (set! sum (+ sum 1))
		       (set! m (1+ m)))
		sum)))
         (day           ; Calculate the day by subtraction
          (- date (1- (calendar-absolute-from-persian
                       (list month 1 year))))))
    (list month day year)))



;;; Mayan Calendars

;; Number of days of the Mayan calendar epoch before absolute day 0.
;; This is the Goodman-Martinez-Thompson correlation used by almost
;; all experts, but some use 1137140.  Using 1232041 gives you
;; Spinden's correlation; using 1142840 gives you Hochleitner's
;; correlation.
(define calendar-mayan-days-before-absolute-zero 1137142)

;; Mayan haab date at the epoch.
(define calendar-mayan-haab-at-epoch '(8 . 18))

;; Mayan tzolkin date at the epoch.
(define calendar-mayan-tzolkin-at-epoch '(4 . 20))

;; Compute the Mayan long count corresponding to the absolute DATE.
(define (calendar-mayan-long-count-from-absolute date)
  (let ((long-count (+ date calendar-mayan-days-before-absolute-zero)))
    (let* ((baktun (quotient long-count 144000))
           (remainder1 (modulo long-count 144000))
           (katun (quotient remainder1 7200))
           (remainder2 (modulo remainder1 7200))
           (tun (quotient remainder2 360))
           (remainder3 (modulo remainder2 360))
           (uinal (quotient remainder3 20))
           (kin (modulo remainder3 20)))
      (list baktun katun tun uinal kin))))

;; Convert absolute DATE into a Mayan haab date (a pair).
(define (calendar-mayan-haab-from-absolute date)
  (let* ((long-count (+ date calendar-mayan-days-before-absolute-zero))
         (day-of-haab
          (modulo (+ long-count
		     (car calendar-mayan-haab-at-epoch)
		     (* 20 (1- (cdr calendar-mayan-haab-at-epoch))))
		  365))
         (day (modulo day-of-haab 20))
         (month (1+ (quotient day-of-haab 20))))
    (cons day month)))

;; Number of days from Mayan haab DATE1 to next occurrence of haab date DATE2.
(define (calendar-mayan-haab-difference date1 date2)
  (mod (+ (* 20 (- (cdr date2) (cdr date1)))
	  (- (car date2) (car date1)))
       365))

;; Absolute date of latest HAAB-DATE on or before absolute DATE.
(define (calendar-mayan-haab-on-or-before haab-date date)
  (- date
     (modulo (- date
		(calendar-mayan-haab-difference
		 (calendar-mayan-haab-from-absolute 0) haab-date))
	     365)))


;; Convert absolute DATE into a Mayan tzolkin date (a pair).
(define (calendar-mayan-tzolkin-from-absolute date)
  (let* ((long-count (+ date calendar-mayan-days-before-absolute-zero))
         (day (calendar-mod
               (+ long-count (car calendar-mayan-tzolkin-at-epoch))
               13))
         (name (calendar-mod
                (+ long-count (cdr calendar-mayan-tzolkin-at-epoch))
                20)))
    (cons day name)))

;; Number of days from Mayan tzolkin DATE1 to next occurrence of
;; tzolkin DATE2.
(define (calendar-mayan-tzolkin-difference date1 date2)
  (let ((number-difference (- (car date2) (car date1)))
        (name-difference (- (cdr date2) (cdr date1))))
    (mod (+ number-difference
	    (* 13 (mod (* 3 (- number-difference name-difference))
		       20)))
	 260)))

;; Absolute date of latest TZOLKIN-DATE on or before absolute DATE.
(define (calendar-mayan-tzolkin-on-or-before tzolkin-date date)
  (- date
     (modulo (- date (calendar-mayan-tzolkin-difference
		      (calendar-mayan-tzolkin-from-absolute 0)
		      tzolkin-date))
	     260)))


;;; 4-4-5 Fiscal Calendar

;; A fiscal calendar loosely based on the ISO calendar in which the
;; year conincides with the Gregorian year, months 1, 2, 4, 5, 7, 8,
;; 10, and 11 end on the saturday of the fourth ISO week in the month,
;; months 3, 6, and 9 end on the Saturday of the 5th ISO week in the
;; month, and month 12 ends on 31 Dec.
;;
;; The day figure counts only weekdays, ignoring holidays.

(define (calendar-445fiscal-month-end month year)
  "Return the day count corresponding to the last day of MONTH in YEAR"
  (if (= month 12)
      (calendar-absolute-from-gregorian (list 12 31 year))
      (calendar-absolute-from-iso (list (+ (* month 4) (quotient month 3)) ;; 4 weeks for each month plus 1 for each quarter end
					6                                  ;; Saturday in the week
					year))))                           ;; the year

(define (calendar-445fiscal-from-absolute date)
  "Return the 4-4-5 fiscal date corresponding to day count DATE"
  (let* ((iso-date (calendar-iso-from-absolute (1+ date)))
	 (g-date (calendar-gregorian-from-absolute date))
	 (day-of-week (modulo date 7))
	 (year (extract-calendar-year g-date))
	 (month (let ((amonth 1))
		  (while (> date (calendar-445fiscal-month-end amonth year))
			 (set! amonth (1+ amonth)))
		  amonth))
	 (day (max (+ (* 5 (max (- (extract-calendar-month iso-date)
				   (* (1- month) 4) 
				   (quotient (1- month) 3) 
				   1)                       ;; 5 days for each week passed this month.
				0))                         ;; If we subtracted more weeks than we have, it must be week 53.
		      (min day-of-week 5)                   ;; Up to 5 days for the current week.
		      (if (and (= month 12) (= (extract-calendar-month iso-date) 1))
			  25
			  0)                                ;; The last couple of days in December can be counted as
		                                            ;;  part of the first week of the next year by ISO.
		      (if (= month 1) 
			  (vector-ref '#(0 0 -1 -2 -3 1 0) 
				      (modulo (calendar-absolute-from-gregorian (list 1 1 year)) 7))
			  0))                               ;; Adjustment for where January 1 lands in the week.
		   1)))                                    ;; Push the first sunday into the first day of the week.
    (list month day year)))

(define (calendar-absolute-from-445fiscal date)
  "Return the day count corresponding to the given 4-4-5 fiscal DATE"
  (let* ((prior-months (1- (extract-calendar-month date)))
	 (year (extract-calendar-year date))
	 (days (if (zero? prior-months)                  ;; Adjust for the beginning of the year.
		   (- (extract-calendar-day date)
		      (vector-ref '#(0 0 -1 -2 -3 1 0)
				  (modulo (calendar-absolute-from-gregorian (list 1 1 year)) 7)))
		   (extract-calendar-day date))))
    (+ (calendar-absolute-from-iso (list (+ (* 4 prior-months) 
					    (quotient prior-months 3) ;; Weeks before this month
					    (quotient (1- days) 5)    ;; Weeks past this month
					    1)                        ;; This week
					 (calendar-mod days 5)
					 year)))))



  (export gregorian-day-name-alist
	  gregorian-month-name-alist
	  calendar-islamic-month-name-array
	  calendar-hebrew-month-name-array-common-year
	  calendar-hebrew-month-name-array-leap-year
	  chinese-calendar-celestial-stem
	  chinese-calendar-terrestrial-branch
	  lunar-phase-name-alist
	  solar-n-hemi-seasons-alist
	  solar-s-hemi-seasons-alist
	  french-calendar-month-name-array
	  french-calendar-multibyte-month-name-array
	  french-calendar-day-name-array
	  french-calendar-multibyte-special-days-array
	  french-calendar-special-days-array
	  coptic-calendar-month-name-array
	  ethiopic-calendar-month-name-array
	  persian-calendar-month-name-array
	  calendar-mayan-haab-month-name-array
	  calendar-mayan-tzolkin-names-array
	  extract-calendar-year
	  extract-calendar-month
	  extract-calendar-day
	  calendar-last-day-of-month
	  calendar-day-number
	  calendar-absolute-from-gregorian
	  calendar-gregorian-from-absolute
	  calendar-nth-named-absday
	  calendar-nth-named-day
	  calendar-dayname-on-or-before
	  calendar-julian-from-absolute
	  calendar-absolute-from-julian
	  calendar-absolute-from-islamic
	  calendar-islamic-from-absolute
	  calendar-hebrew-from-absolute
	  calendar-absolute-from-hebrew
	  calendar-absolute-from-chinese
	  calendar-chinese-from-absolute
	  calendar-absolute-from-french
	  calendar-french-from-absolute
	  calendar-absolute-from-coptic
	  calendar-coptic-from-absolute
	  calendar-absolute-from-ethiopic
	  calendar-ethiopic-from-absolute
	  calendar-absolute-from-iso
	  calendar-iso-from-absolute
	  calendar-absolute-from-persian
	  calendar-persian-from-absolute
	  calendar-mayan-long-count-from-absolute
	  calendar-mayan-haab-from-absolute
	  calendar-mayan-tzolkin-from-absolute
	  calendar-445fiscal-from-absolute
	  calendar-absolute-from-445fiscal)
