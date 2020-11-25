;;;; parse.scm --- Functions for parsing a date string.
;; Copyright (C) 2002, 2013 Daniel P. Valentine
;;
;; Author: Daniel P. Valentine <dpv@world.std.com>

;; parse.scm is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 3, or (at your option)
;; any later version.
  
;; parse.scm is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with calendar.scm; see the file COPYING.  If not, write to the
;; Free Software Foundation, Inc., 59 Temple Place - Suite 330,
;; Boston, MA 02111-1307, USA. 

(define-module (combine_scm parse))

(use-modules (combine_scm calendar))

;;; Public ways of affecting the parse

(define parse-date-expected-order '(("YYYY" "MM" "DD") ("DD" "MM" "YYYY") ("MM" "DD" "YYYY")))

(define parse-date-method-preference '(("8digits" "julian-gregorian")
				       ("delimited" "julian-gregorian")
				       ("word-month" "julian-gregorian")))
(define parse-date-delimiters '(#\/ #\- #\. #\space))
(define parse-date-gregorian-start '(9 24 1752))

(define parse-date-gregorian-month-name-table '( ("january" 1)
						 ("february" 2)
						 ("march" 3)
						 ("april" 4)
						 ("may" 5)
						 ("june" 6)
						 ("july" 7)
						 ("august" 8)
						 ("september" 9)
						 ("october" 10)
						 ("november" 11)
						 ("december" 12)))

(define parse-date-month-name-table (list (list "gregorian" parse-date-gregorian-month-name-table)
					  (list "julian-gregorian" parse-date-gregorian-month-name-table)))

(export parse-date-expected-order
	parse-date-method-preference
	parse-date-delimiters
	parse-date-gregorian-start
	parse-date-name-table)
;;; Public parse functions

(define (parse-date date-string output-format)
  "Read a DATE-STRING and return the date interpreted and in OUTPUT-FORMAT"
  (let ((work-preference parse-date-method-preference)
	(result #f))
    (while (and (not (null? work-preference)) (not result))
	   (set! result (local-eval (list (string->symbol (string-append "parse-date-" (caar work-preference)))
					  date-string (cadar work-preference)) (the-environment)))
	   (set! work-preference (cdr work-preference)))
    (local-eval (list (string->symbol (string-append "calendar-" output-format "-from-absolute")) result) (the-environment))))

(export parse-date)

;;; Private parse functions

(define (parse-date-8digits string calendar)
  (if (not (= (string-length string) 8))
      #f
      (let ((expected-order parse-date-expected-order)
	    (result #f))
	(while (and (not result) (not (null? expected-order)))
	       (set! result (local-eval (list (string->symbol (string-append "parse-date-" calendar))
					      '(parse-date-split-fixed string (car expected-order))) (the-environment)))
	       (set! expected-order (cdr expected-order)))
	result)))

(define (parse-date-delimited string calendar)
  (let ((expected-order parse-date-expected-order)
	(result #f))
    (while (and (not result) (not (null? expected-order)))
	   (set! result (local-eval (list (string->symbol (string-append "parse-date-" calendar))
					  '(parse-date-split-delimited string (car expected-order))) (the-environment)))
	   (set! expected-order (cdr expected-order)))
    result))

(define (parse-date-word-month string calendar)
  (local-eval (list (string->symbol (string-append "parse-date-" calendar))
		    '(parse-date-split-word-month string calendar)) (the-environment)))

(define (parse-date-julian-gregorian date)
  (let ((year (extract-calendar-year date))
	(month (extract-calendar-month date))
	(day (extract-calendar-day date)))
    (if (or (not year)
	    (not month)
	    (not day))  ;; Check for invalid date parts.
	#f
	(let ((g-date (calendar-absolute-from-gregorian date)))
	  (if (>= g-date (calendar-absolute-from-gregorian parse-date-gregorian-start)) ;; We're good with Gregorian 
	      (if (or (not (< 0 month 13))
		      (not (<= 1 day (calendar-last-day-of-month month year))))
		  #f ;; Out of range
		  g-date)
	      (if (or (not (< 0 month 13))
		      (not (or (<= 1 day (calendar-last-day-of-month month year))
			       (and (= month 2) (= day 29) (= (modulo year 4) 0)))))
		  #f
		  (calendar-absolute-from-julian date)))))))

(define (parse-date-split-fixed string expected-order)
  (let ((work-order expected-order)
	(work-string string)
	(result (make-vector 3 0)))
    (while (and (not (null? work-order)) (> (string-length work-string) 0))
	   (cond ((equal? (car work-order) "YYYY") (begin (vector-set! result 2 (string->number (substring work-string 0 4)))
							  (set! work-string (substring work-string 4 (string-length work-string)))))
		 ((equal? (car work-order) "MM") (begin (vector-set! result 0 (string->number (substring work-string 0 2)))
							(set! work-string (substring work-string 2 (string-length work-string)))))
		 ((equal? (car work-order) "DD") (begin (vector-set! result 1 (string->number (substring work-string 0 2)))
							(set! work-string (substring work-string 2 (string-length work-string))))))
	   (set! work-order (cdr work-order)))
    (vector->list result)))

;; This should be replaced by another library function once I have
;; access to the later Guile
(define (parse-date-string-tokenize string delimiter-list)
  (let ((char-list (string->list string))
	(current-token '())
	(result-list '()))
    (while (not (null? char-list))
	   (if (memv (car char-list) delimiter-list)
	       (begin (set! result-list (cons (list->string (reverse current-token)) result-list))
		      (set! current-token '())
		      (set! char-list (cdr char-list)))
	       (begin (set! current-token (cons (car char-list) current-token))
		      (set! char-list (cdr char-list)))))
    (set! result-list (cons (list->string (reverse current-token)) result-list))
    (reverse result-list)))


(define (parse-date-split-delimited string expected-order)
  (let ((work-order expected-order)
	(work-list (parse-date-string-tokenize string parse-date-delimiters))
	(result (make-vector 3 0)))
    (while (and (not (null? work-order)) (not (null? work-list)))
	   (cond ((equal? (car work-order) "YYYY") (vector-set! result 2 (string->number (car work-list))))
		 ((equal? (car work-order) "MM") (vector-set! result 0 (string->number (car work-list))))
		 ((equal? (car work-order) "DD") (vector-set! result 1 (string->number (car work-list)))))
	   (set! work-list (cdr work-list))
	   (set! work-order (cdr work-order)))
    (vector->list result)))

(define (parse-date-trim-punct string)
  (let ((length (string-length string)))
    (if (or (equal? (substring string (1- length)) ".")
	    (equal? (substring string (1- length)) ","))
	(substring string 0 (1- length))
	string)))

(define (parse-date-split-word-month string calendar)
  (let ((work-list (parse-date-string-tokenize string '(#\space #\tab)))
	(result (make-vector 3 0)))
    (while (not (null? work-list))
	   (let* ((candidate (list->string (map char-downcase (string->list (parse-date-trim-punct (car work-list))))))
		  (month (assoc candidate (cadr (assoc calendar parse-date-month-name-table))))
		  (number (string->number candidate)))
	     (cond (month (vector-set! result 0 (cadr month)))
		   ((= 0 (vector-ref result 1)) (vector-set! result 1 number))
		   (#t (vector-set! result 2 number))))
	   (set! work-list (cdr work-list)))
    (vector->list result)))

(export parse-date-split-fixed
	parse-date-split-delimited
	parse-date-julian-gregorian)

(re-export
	extract-calendar-month
	extract-calendar-year
	extract-calendar-day)

