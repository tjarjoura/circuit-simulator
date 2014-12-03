CIRCUIT SIMULATOR
-----------------

This program allows you to describe circuits using a lisp-like language, and then evaluate them given a set of inputs.

*Gates supported*
1. NOT

2. OR

3. AND

4. T-FLIPFLOP 

*Language description*
To describe a gate, the syntax is as follows:

(GATE-NAME INPUTS)

Where each INPUT is either a label, another gate, or an input variable name. 'and' gates and 'or' gates take two inputs, while 'not' gates and 'tflipflop' gates take one input.

A label can be defined as followed:

(LABEL-NAME VALUE)

Where VALUE is either another label, a gate, or an input variable.

*Usage*
First give a line of input for the names of each input variable. For example 'p q s' would define 3 input variables with the names 'p', 'q', and 's' respectively.

Next give a line describing the circuit. Use the syntax above. For example, (or (and p q) s) describes a circuit that takes the output from an 'and' gate and the 's' input variable and connects them to the input of an 'or' gate. The output of the circuit is the output of the outermost gate. Define all of your labels within this circuit definition. Label definitions can be used as inputs.

Finally give a set of lines containing values(0 or 1) for each of the input variables. There should be one on each line for each input variable given in the first line.  
-----------------

This program allows you to describe circuits using a lisp-like language, and then evaluate them given a set of inputs.

*Gates supported*
1. NOT

2. OR

3. AND

4. T-FLIPFLOP 

*Language description*
To describe a gate, the syntax is as follows:

(GATE-NAME INPUTS)

Where each INPUT is either a label, another gate, or an input variable name. 'and' gates and 'or' gates take two inputs, while 'not' gates and 'tflipflop' gates take one input.

A label can be defined as followed:

(LABEL-NAME VALUE)

Where VALUE is either another label, a gate, or an input variable.

*Usage*
First give a line of input for the names of each input variable. For example 'p q s' would define 3 input variables with the names 'p', 'q', and 's' respectively.

Next give a line describing the circuit. Use the syntax above. For example, (or (and p q) s) describes a circuit that takes the output from an 'and' gate and the 's' input variable and connects them to the input of an 'or' gate. The output of the circuit is the output of the outermost gate. Define all of your labels within this circuit definition. Label definitions can be used as inputs.

Finally give a set of lines containing values(0 or 1) for each of the input variables. There should be one on each line for each input variable given in the first line. The program will output the output value from the final run.

For example:

p q r
(and (or p q) (not r))
0 1 0 
will evaluate to 1.

p q r
(and (tflipflop mylabel) (or (mylabel (not p))  (and q r)))
0 1 0
1 1 1

will create a label named 'mylabel' and have it refer to the output from (not p). This way we can send the output of this gate to more than one input(in this case a 'tflipflop' gate and an 'or' gate).
The output from this will be 1.
