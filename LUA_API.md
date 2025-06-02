# Soup Lua Bindings

Although the Lua bindings themselves are compatible with vanilla Lua, some of the code samples provided here require [Pluto](https://plutolang.github.io/docs/Introduction/).

## I/O

> [!WARNING]
> These functions can easily be used incorrectly so new code is highly advised to use [pluto-zip](<https://github.com/PlutoLang/pluto-zip>) instead.

### *userdata* soup.FileReader(*string* path)

### *userdata* soup.StringReader(*string* data)

### *userdata* soup.ZipReader(*userdata* reader)

Note that the ZipReader takes a pointer to the reader instance, so a ZipReader instance must not reach scopes that the reader instance can't reach.

```Lua
local fr = soup.FileReader("test.zip")
local zr = soup.ZipReader(fr)
for _, f in zr:getFileList() do
    print(f.name .. ": " .. zr:getFileContents(f))
end
```

## Math

### *userdata* soup.Matrix()

Returns a soup::Matrix instance which provides `setPosRotXYZ` method and multiplication operator taking soup::Vector3.

<h3>
    <i>userdata</i> soup.Vector3()<br>
    <i>userdata</i> soup.Vector3(<i>number</i> x, <i>number</i> y, <i>number</i> z)
</h3>

Returns a soup::Vector3 instance.
