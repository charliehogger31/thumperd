Web server with embedded brainfuck support.
### Example
```html
<html>
<body>
<p>
$--------[-->+++<]>.--.+++.---------.-[--->+<]>.-[---->+<]>++.[->+++<]>+.-[->+++<]>.--[->++++<]>-.--------.+++.-------.--.+++++.-------.-[--->+<]>--.[-->+++++++<]>.-------.----.--.++.--[--->+<]>-.+++++[->+++<]>.---------.[--->+<]>--.[->+++<]>+.--[--->+<]>---.++.$
</p>
</body>
</html>
```
BF between the '$' will be evaluated. The . operator is equivalent to the php's ```echo```.
Uses custom 'thumper' bf engine. Name inspired by a certain JS engine.
Don't use this in production