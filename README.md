# NFA
Read from file consisting of a tuple and output whether the sting is accepted by NFA or not.

file is formated consisting of a tuple (α, β) where α is a tuple for an NFA as
explained in Section 2.1 below, and β is a tuple (t1, . . . , tn) where for i ∈ 1..n, ti
is a string over the alphabet of the NFA.

if β is empty tuple (), this program will ask user to type from the keyboard an input string
and display whether the input string is accepted or rejected, and repeat this process until
the user exits the program (as shown in the execution example below). 

If β is not empty, this program display a tuple of format (r1, . . . , rn) where for i ∈ 1..n, ri
is accepted if the string is accepted by the NFA, or rejected otherwise.
