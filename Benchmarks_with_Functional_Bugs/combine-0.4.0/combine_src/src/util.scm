;;; util.scm - base extension functions for GNU combine
;;;          - This program can be loaded directly with
;;;            combine's --extensioninitfile (-X) option or
;;;            it can be included in a user's initialization
;;;            file.

;;; General predicates (in addition to the standards)
;;;
;;; These, along with the standard predicates are appropriate for
;;; match and filter extensions (where match sees a reference and a
;;; data record and filter sees only a single input record).  In
;;; general a true result means to keep going and a false (#f) means
;;; to stop considering the reference or match.

;;; Type predicates
;;;
;;; The standard gives us eqv?, eq?, and equal? for equivalence.  It
;;; gives us the type checkers list?, pair?, symbol?, number?,
;;; complex?, real?, rational?, integer?, char?, string?, vector?,
;;; procedure?.  So far I have seen no need to expand.

;;; Value predicates
;;;
;;; It gives us the value checkers null?, exact?, inexact?, zero?,
;;; positive?, negative?, odd?, even?, char-alphabetic?,
;;; char-numeric?, char-whitespace?, char-upper-case?,
;;; char-lower-case?.


;;; Comparison predicates
;;;
;;; It gives us the comparison predicates =, <, >, <=, >=, char=?,
;;; char<?, char>?, char<=?, char>=?, char-ci=?, char-ci<?, char-ci>?,
;;; char-ci<=?, char-ci>=?, string=?, string<?, string>?, string<=?,
;;; string>=?, string-ci=?, string-ci<?, string-ci>?, string-ci<=?,
;;; string-ci>=?.

;;; between-excl? -- Determine if a number is between two others, not
;;;   counting the end points.
;;;
;;;   Arguments: 
;;;     num -- the number to be tested 
;;;     low -- the lower bound
;;;     high -- the upper bound

(define (between-excl? num low high)
  (and (> num low) (< num high)))

;;; between-incl? -- Determine if a number is between two others,
;;; counting the end points.
;;;
;;;   Arguments: 
;;;     num -- the number to be tested 
;;;     low -- the lower bound
;;;     high -- the upper bound

(define (between-incl? num low high)
  (and (>= num low) (<= num high))) 


;;; String manipulation
;;
;; Someone has probably already written these better, but I did them anyway.

;; rpad --- pad STRING on the right to a given LENGTH with CHARACTER.
;; If CHARACTER is not given, spaces are used.
(define (rpad string length . character)
  (let ((padlength (max (- length (string-length string)) 0))
	(padchar (if (null? character) #\space (car character))))
    (string-append string (make-string padlength padchar))))

;; lpad --- pad STRING on the left to a given LENGTH with CHARACTER.
;; If CHARACTER is not given, spaces are used.
(define (lpad string length . character)
  (let ((padlength (max (- length (string-length string)) 0))
	(padchar (if (null? character) #\space (car character))))
    (string-append (make-string padlength padchar) string)))

;; trim --- Trim spaces from either end of string
(define (trim string)
  (let ((char-list (string->list string)))
    (while (and (not (null? char-list)) (equal? (car char-list) #\space))
	   (set! char-list (cdr char-list)))
    (let ((rev-list (reverse char-list)))
      (while (and (not (null? rev-list)) (equal? (car rev-list) #\space))
	     (set! rev-list (cdr rev-list)))
      (if (null? rev-list) (set! rev-list (list #\0)))
      (list->string (reverse rev-list)))))



;;; Unit Conversion

(define (to-grams amount unit)
  (cond ((equal? unit "KG") (* amount 1000))
	((equal? unit "G") amount)
	(#t #f)))



;;; Date Conversion
;;;
;;; Dates can be represented in many forms and in many sytems, but in
;;; essence they are a way of counting the days.  The general idea of
;;; this date comparison section is to provide procedures that convert
;;; dates to and from a standard day count.  This day count can then
;;; be used for calculations of numbers of days and for comparisons of
;;; previously unlike dates.  I may also want to include a similar
;;; month count and year count so that people can work in bigger
;;; chunks.


(use-modules (combine_scm parse))


;;; Convert from and to fixed gregorian formats.  The following
;;; functions convert to and from gregorian formats with 4-digit years
;;; and 2-digit days and months, with or without delimiters.

;;; YYYYMMDD->daycount -- convert a date with a 4-digit year, 2-digit
;;; month, and 2-digit day into the standard day count.  The argument
;;; is a string in that format and may be delimited with any
;;; non-numeric character or none.

(define (YYYYMMDD->daycount datestring)
  (let* ((year-plus (date-get-4year datestring))
	 (month-day (date-skip-delimiter (cadr year-plus) date-normal-delimiters))
	 (month-plus (date-get-2digit month-day))
	 (day (date-skip-delimiter (cadr month-plus) date-normal-delimiters))
	 (day-plus (date-get-2digit day)))
    (calendar-absolute-from-gregorian (list (car month-plus) (car day-plus) (car year-plus)))))

;;; MMDDYYYY->daycount -- convert a date with a 2-digit month, 2-digit
;;; day, and 4-digit year into the standard day count.  The argument
;;; is a string in that format and may be delimited with any
;;; non-numeric character or none.

(define (MMDDYYYY->daycount datestring)
  (let* ((month-plus (date-get-2digit datestring))
	 (day-year (date-skip-delimiter (cadr month-plus) date-normal-delimiters))
	 (day-plus (date-get-2digit day-year))
	 (year (date-skip-delimiter (cadr day-plus) date-normal-delimiters))
	 (year-plus (date-get-4year year)))
    (calendar-absolute-from-gregorian (list (car month-plus) (car day-plus) (car year-plus)))))

;;; DDMMYYYY->daycount -- convert a date with a 2-digit day, 2-digit
;;; month, and 4-digit year into the standard day count.  The argument
;;; is a string in that format and may be delimited with any
;;; non-numeric character or none.

(define (DDMMYYYY->daycount datestring)
  (let* ((day-plus (date-get-2digit datestring))
	 (month-year (date-skip-delimiter (cadr day-plus) date-normal-delimiters))
	 (month-plus (date-get-2digit month-year))
	 (year (date-skip-delimiter (cadr day-plus) date-normal-delimiters))
	 (year-plus (date-get-4year year)))
    (calendar-absolute-from-gregorian (list (car month-plus) (car day-plus) (car year-plus)))))


;;; daycount->YYYYMMDD --- convert an absolute day count to a string
;;; with a year, a month, and a day with an optional delimiter
(define (daycount->YYYYMMDD daycount . delimiter)
  (let* ((datelist (calendar-gregorian-from-absolute daycount))
	 (year (lpad (number->string (extract-calendar-year datelist)) 4 #\0))
	 (month (lpad (number->string (extract-calendar-month datelist)) 2 #\0))
	 (day (lpad (number->string (extract-calendar-day datelist)) 2 #\0)))
    (if (null? delimiter)
	(string-append year month day)
	(string-append year (car delimiter) month (car delimiter) day))))

;;; daycount->MMDDYYYY --- convert an absolute day count to a string
;;; with a year, a month, and a day with an optional delimiter
(define (daycount->MMDDYYYY daycount . delimiter)
  (let* ((datelist (calendar-gregorian-from-absolute daycount))
	 (year (lpad (number->string (extract-calendar-year datelist)) 4 #\0))
	 (month (lpad (number->string (extract-calendar-month datelist)) 2 #\0))
	 (day (lpad (number->string (extract-calendar-day datelist)) 2 #\0)))
    (if (null? delimiter)
	(string-append month day year)
	(string-append month (car delimiter) day (car delimiter) year))))

;;; daycount->DDMMYYYY --- convert an absolute day count to a string
;;; with a year, a month, and a day with an optional delimiter
(define (daycount->DDMMYYYY daycount . delimiter)
  (let* ((datelist (calendar-gregorian-from-absolute daycount))
	 (year (lpad (number->string (extract-calendar-year datelist)) 4 #\0))
	 (month (lpad (number->string (extract-calendar-month datelist)) 2 #\0))
	 (day (lpad (number->string (extract-calendar-day datelist)) 2 #\0)))
    (if (null? delimiter)
	(string-append day month year)
	(string-append day (car delimiter) month (car delimiter) year))))


;;; Date Conversion Utility Functions

;;; date-normal-delimiters -- A list of standard delimiters that are
;;; used between fields in date strings so that the user does not
;;; always have to think of them.
(define date-normal-delimiters (list #\space #\/ #\- #\.))

;;; date-skip-delimiter -- Skip over potential delimiters, returning
;;; the rest of the string after delimiters are past.  Takes the
;;; string as an argument, followed by a list of the possible
;;; delimiters.

(define (date-skip-delimiter datestring delimiters)
  (if (null? delimiters) datestring
      (do ((stringchars (string->list datestring) (cdr stringchars)))
	  ((or (null? stringchars)
	       (not (member (car stringchars) delimiters)))
	   (list->string stringchars)))))

;;; date-get-4year -- extract the 4-digit year from a string returning
;;; a list of the year as a number and the rest of the string

(define (date-get-4year datestring)
  (list (string->number (substring datestring 0 4))
	(substring datestring 4 (string-length datestring))))

;;; date-get-2year -- extract the 2-digit year from a string returning
;;; a list of the year as a number and the rest of the string The
;;; century is a required parameter.  The cut-off year (the last
;;; 2-digit year to be considered part of the stated century) is
;;; optional.

(define (date-get-2year datestring century . restargs)
  (list (let ((shortyear (string->number (substring datestring 0 2)))
	      (cut-off (if (null? restargs) 99 (car restargs))))
	  (if (> shortyear cut-off)
	      (+ (- century 100) 
		 (string->number (substring datestring 0 2)))
	      (+ century 
		 (string->number (substring datestring 0 2)))))
	  (substring datestring 2 (string-length datestring))))

;;; date-get-2digit -- extract a 2-digit number from a string and
;;; return a list of the number and the rest of the string.
(define (date-get-2digit datestring)
  (list (string->number (substring datestring 0 2))
	(substring datestring 2 (string-length datestring))))
