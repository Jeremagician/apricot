#lang racket

(define (factorial n) 
	(if (= n 0) 1
		(factorial (* n (factorial (- n 1))))
	)
)
