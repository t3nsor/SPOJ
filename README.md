SPOJ
====

This repository contains solutions to almost all problems I have solved on
SPOJ. They're intended as a last resort in case you've tried very hard to solve
a problem but you just haven't been able to figure it out. When I was trying to
solve <a href="http://www.spoj.com/problems/GSS2/">GSS2</a>, I mostly found
vague hints on online forums, which weren't helpful.  I wasn't able to solve
the problem until after I looked at some code. So obviously the code helps, and
there can be good reasons to just look at code rather than being stuck
indefinitely.

Some of the files in this repository differ from my submitted solutions in
relatively trivial ways, such as the removal of commented-out code or
`#ifdef`'d code for an archaic compiler I used to use (Microsoft Visual C++
6.0). However, I suspect some of them simply don't work because either they
contained errors that I corrected while away from home (so the changes were
not saved locally) or I made modifications locally that turned out to give
incorrect solutions and forgot to revert to the older versions. If you discover
an incorrect solution, please leave a comment so I can fix it!

The copyright notice (LICENSE) applies to all code except for a few algorithms
copied from the Stanford ACM team notebook, which are indicated as such. There
is technically no license for that code that I'm aware of, but it's unlikely
you'll get into any trouble for using it or redistributing it freely. A recent
version of the Stanford ACM team notebook can be found <a
href="http://stanford.edu/~liszt90/acm/notebook.html">here</a>.

There is a comment at the beginning of each file that gives the date on which
the solution was submitted. Caution: Many of the older solutions have bad
coding style and poor mastery of C++.

For a few problems I have multiple solutions, usually because after my first
successful submission I discovered there was a much better way to solve the
problem. Those are indicated as e.g. niceday-1.cpp and niceday-2.cpp. There is
no possible ambiguity as SPOJ problem names aren't allowed to contain dashes
(as far as I know, anyway).
