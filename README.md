Web server with embedded brainfuck support. Now supports plugins.
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
### Plugins
See example plugin for code.
'handoff' struct contains pointers to engine state internals. Plugins effect the engine by modifying these.
The program knows what to load by the list file defined in the environment variable: TE_LIST_FILE.
The order in which the plugins are loaded effect the 'interrupt vector' which they can be called from.
In order for brainfuck programs to jump to plugin code they must have the vector in the current cell then use the '!' operator to jump to the plugin entry point.
Plugins must be able to be loaded by dlopen. Plugins should be thread-safe since they can be invoked by many connection threads at once.
> Note: COBOL can be used to create plugins.
## Environment Variables
You can configure the web server through different environment variables.
- **TE_USE_TLS**: If it is set to "YES" the server will use TLS.
- **TE_TLS_CERT & TE_TLS_KEY**: Must be set to the certificate and key files respectively. Must be set in order to use TLS.
- **TE_PORT**: Port variable, set to a port you want the server to listen on. Default is 8888.
- **TE_LIST_FILE**: The plugin list file location, should point to a file that contains a list of plugins to be loaded. If it is not set plugins won't be enabled.
- **TE_DEF_PATH**: wwwroot path for serving content. Default it "."