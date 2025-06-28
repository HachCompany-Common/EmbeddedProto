# Version Mismatch Detection in Embedded Proto

## Overview

Embedded Proto v4 introduces version mismatch detection to prevent compatibility issues between the core library and generated message files. This feature ensures that users are notified of potential incompatibilities at compile time.

## Version Format

Embedded Proto follows semantic versioning (X.Y.Z):
- X: Major version
- Y: Minor version
- Z: Patch version

## Version Checking

The system performs different levels of checks based on the version component:

1. **Major Version (X)**
   - Mismatches trigger a compiler error via static_assert
   - Major version changes indicate breaking API changes
   - Example error: "Major version mismatch between generated code (3.0.0) and library (4.0.0)"

2. **Minor Version (Y)**
   - Mismatches trigger a compiler warning
   - Minor version changes indicate new features with backward compatibility
   - Example warning: "Minor version mismatch between generated code (4.1.0) and library (4.0.0)"

3. **Patch Version (Z)**
   - Mismatches are ignored
   - Patch version changes indicate bug fixes only

## Implementation Details

### Core Library Version
The core library version is defined in `src/Version.h`:
```cpp
namespace EmbeddedProto {
    constexpr char VERSION_MAJOR = 4;
    constexpr char VERSION_MINOR = 0;
    constexpr char VERSION_PATCH = 0;
    constexpr const char* VERSION_STRING = "4.0.0";
}
```

### Generated Code Version
Each generated header file includes version information:
```cpp
#define EMBEDDEDPROTO_VERSION_MAJOR 4
#define EMBEDDEDPROTO_VERSION_MINOR 0
#define EMBEDDEDPROTO_VERSION_PATCH 0
```

## Resolving Version Conflicts

### Major Version Mismatch
If you encounter a major version mismatch:
1. Check the changelog for breaking changes
2. Either:
   - Regenerate message files using the matching major version
   - Or upgrade/downgrade the library to match your generated code

### Minor Version Mismatch
If you receive a minor version mismatch warning:
1. Review the changelog for new features or deprecations
2. Consider regenerating message files to utilize new features
3. If regeneration isn't feasible, you can safely continue using the current version

### Patch Version Mismatch
- No action required
- Patch version mismatches indicate bug fixes only
- Generated code will continue to work correctly

## Best Practices

1. **Version Control**
   - Keep track of which Embedded Proto version generated your message files
   - Document the version in your project's dependency management

2. **Continuous Integration**
   - Add version compatibility checks to your CI pipeline
   - Regenerate message files when upgrading Embedded Proto

3. **Upgrade Strategy**
   - Plan major version upgrades carefully
   - Test thoroughly after any version change
   - Keep all generated files on the same version

## Example

```cpp
// Library version (Version.h)
namespace EmbeddedProto {
    constexpr char VERSION_MAJOR = 4;
    constexpr char VERSION_MINOR = 0;
    constexpr char VERSION_PATCH = 0;
}

// Generated code version
#define EMBEDDEDPROTO_VERSION_MAJOR 4  // Must match
#define EMBEDDEDPROTO_VERSION_MINOR 0  // Warning if mismatch
#define EMBEDDEDPROTO_VERSION_PATCH 1  // Ignored if mismatch
```

## Further Information

For more details about version compatibility and upgrade guides, visit:
- [Embedded Proto Documentation](https://embeddedproto.com/documentation/)
- [Release Notes](https://embeddedproto.com/releases/)
- [Migration Guides](https://embeddedproto.com/migrations/)
