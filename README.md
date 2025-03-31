# C-Rust

A C library that implements Rust-like error handling and optional value patterns.

## Features

- `Result<T, E>` pattern for error handling
- `Option<T>` pattern for nullable values
- Memory-safe error handling
- Comprehensive test coverage
- Example implementations

## Building

```bash
make all        # Build everything
make test       # Run tests
make app        # Build example application
```

## Usage Example

```c
#include <c_rust.h>

typedef struct Point {
  int x;
  int y;
} Point;

Result point_new(int x, int y) {
  Point* p = malloc(sizeof(*p));
  if (p == NULL) {
    return result_error(ERROR_CODE_GENERAL, "Failed to allocate Point");
  }
  p->x = x;
  p->y = y;
  return result_ok(p);
}

int main() {
  Result res = point_new(2, 2);
  if (result_is_error(&res)) {
    fprintf(stderr, "Error: %s\n", res.error.message);
    return 1;
  }
  
  Point* p = res.ok.value;
  printf("(%d, %d)\n", p->x, p->y);
  free(p);
  
  return 0;
}
```

## Error Handling

The library provides several ways to handle errors:

1. Using `Result` type:
   ```c
   Result res = some_function();
   if (result_is_error(&res)) {
     // Handle error
     return;
   }
   // Use res.ok.value
   ```

2. Using `Option` type:
   ```c
   Option opt = some_optional_function();
   if (option_is_some(&opt)) {
     // Use opt.some.value
   }
   ```

## Testing

The library includes comprehensive tests covering:
- Basic functionality
- Error cases
- Edge cases
- Memory safety
- Error propagation patterns

Run tests with:
```bash
make test
./build/bin/test
```

## License

MIT License
