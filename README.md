# C-Rust

A C library that implements Rust-like error handling and optional value patterns. The library provides a safe and ergonomic way to handle errors and optional values in C, inspired by Rust's `Result` and `Option` types.

## Features

- `Result<T, E>` pattern for error handling
- `Option<T>` pattern for nullable values
- Standard error integration with errno
- Memory-safe error handling with compile-time assertions
- Comprehensive test suite with 100% coverage

## Dependencies

- C compiler (gcc recommended)
- Make
- Criterion (for running tests)

## Building

```bash
make all        # Build everything
make test       # Run tests
make app        # Build example application
```

## Usage Examples

### Result Type

1. Basic Success/Error Handling:
```c
Result point_new(int x, int y) {
    Point* p = malloc(sizeof(*p));
    if (p == NULL) {
        return result_error(ERROR_CODE_GENERAL, "Failed to allocate Point");
    }
    p->x = x;
    p->y = y;
    return result_ok(p);
}

// Usage
Result res = point_new(2, 2);
if (result_is_error(&res)) {
    fprintf(stderr, "Error: %s\n", res.error.message);
    return 1;
}
Point* p = res.ok.value;
```

2. Void Results (for operations that can fail but return no value):
```c
Result print_point(const Point* p) {
    if (p == NULL) {
        return result_error(ERROR_CODE_GENERAL, "Null pointer");
    }
    printf("(%d, %d)\n", p->x, p->y);
    return result_void();
}

// Usage
Result res = print_point(p);
if (result_is_void(&res)) {
    // Operation completed successfully
}
```

3. System Error Integration:
```c
Result read_file(const char* path) {
    FILE* f = fopen(path, "r");
    if (f == NULL) {
        return result_std_error();  // Uses errno automatically
    }
    // ... rest of the code
}
```

### Option Type

1. Handling Optional Values:
```c
Option find_value(const char* key) {
    void* value = lookup(key);
    if (value == NULL) {
        return option_none();
    }
    return option_some(value);
}

// Usage
Option opt = find_value("key");
if (option_is_some(&opt)) {
    void* value = opt.some;
    // Use value
}
```

## Error Handling Guidelines

1. Use `Result` for operations that can fail
2. Use `Option` for values that might be absent
3. Always check return values:
   - `result_is_ok()` for successful operations with values
   - `result_is_void()` for successful operations without values
   - `result_is_error()` for error conditions
   - `option_is_some()` for present optional values
   - `option_is_none()` for absent optional values

## Testing

The library includes comprehensive tests covering:
- Error creation and handling
- Result type operations (ok, void, error)
- Option type operations (some, none)
- Standard error integration
- Edge cases (empty strings)
- Memory safety assertions

Run tests with:
```bash
make test
./build/bin/test
```

Current test coverage: 33 tests covering all functionality.

## Safety Notes

1. NULL pointer safety:
   - Functions assert on NULL inputs
   - Use Option type for nullable values
   - Use Result type for operations that can fail

2. Memory safety:
   - Always free allocated memory
   - Use Result to handle allocation failures
   - Check error conditions before using values

## License

MIT License
