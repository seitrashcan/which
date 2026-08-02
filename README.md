# mhx::which

`which<T, E>` is a tagged union for fallible operations. It holds either a
success value of type `T` or an error value of type `E` - exactly one, always
initialized, never reassigned.


## Interface

```cpp
mhx::which<Config, ParseError> load(std::string_view path);

auto [cfg, err] = load(path);
if (err) { handle(*err); return; }
use(*cfg);
```

Both bindings are pointers. Exactly one is non-null. Construction is always
explicit - no implicit conversion from `T` or `E`.

```cpp
auto w = mhx::which<int, std::string>::ok(42);
auto w = mhx::which<void, std::string>::ok();
auto w = mhx::which<int, std::string>::err("something went wrong");
```

Direct access is available when only one alternative is relevant. Accessing the
wrong alternative is undefined behavior in release builds, guarded by `assert()`
in debug builds - the same contract as `operator[]` on standard containers.

```cpp
if (w.is_ok())  use(w.value());
if (w.is_err()) log(w.error());
if (w)          use(*w);
```

Chaining fallible operations:

```cpp
mhx::which<Config, Error> load(std::string_view path) {
    auto [data, e1] = read_file(path);
    if (e1) return mhx::which<Config, Error>::err(*e1);

    auto [parsed, e2] = parse_toml(*data);
    if (e2) return mhx::which<Config, Error>::err(*e2);

    auto [validated, e3] = validate(*parsed);
    if (e3) return mhx::which<Config, Error>::err(*e3);

    return mhx::which<Config, Error>::ok(*validated);
}
```


## Comparison with `std::expected`

| | `mhx::which<T, E>` | `std::expected<T, E>` |
|---|---|---|
| How you read a result | `auto [val, err] = f();` | `auto r = f(); r.value(); r.error();` |
| How you signal success | `which<T,E>::ok(v)` | Implicit: return a `T` directly |
| How you signal failure | `which<T,E>::err(e)` | `return std::unexpected(e)` |
| Bad access | UB, assert in debug | Throws `bad_expected_access` |
| Error propagation | Early return with `if (err)` | `.and_then()` chains or early return |
| Reassignment | Not allowed | Allowed |
| Exception consistency | Full | Partial (`bad_expected_access` throws) |


## Codegen

No overhead over a hand-rolled tagged struct. Under Clang at `-O2`, a two-branch
fallible function over a trivial error type compiles to 5 instructions on the
success path and 6 on the error path, with direct construction into the caller's
return slot and no intermediate copies. GCC produces equivalent code.

The `std::variant` backing store pulls in the Itanium EH personality declaration
at the IR level, but no landing pads are generated since nothing inside `which<>`
throws.


## ABI versioning

All types live in `mhx::v1` via an inline namespace, exposed as `mhx`. Future
breaking changes increment the version; old binaries remain link-compatible.


## Requirements

- C++23


## License

Apache-2.0.
