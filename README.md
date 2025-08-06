# Cub3D - A 3D Raycasting Game Engine

![Cub3D Banner](https://img.shields.io/badge/42-cub3d-black?style=flat-square)
![Language](https://img.shields.io/badge/Language-C-blue.svg)
![MLX](https://img.shields.io/badge/Graphics-MLX-green.svg)

## 📖 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Technical Architecture](#technical-architecture)
- [Installation](#installation)
- [Usage](#usage)
- [Controls](#controls)
- [Map Configuration](#map-configuration)
- [Project Structure](#project-structure)
- [Algorithm Details](#algorithm-details)
- [Performance Features](#performance-features)
- [Contributing](#contributing)
- [Authors](#authors)

## 🎯 Overview

Cub3D is a **3D raycasting game engine** inspired by the classic **Wolfenstein 3D**. This project demonstrates the fundamentals of 3D graphics rendering using the **raycasting technique** to create a pseudo-3D perspective from a 2D map. Built entirely in **C** using the **MinilibX graphics library**, this project showcases advanced programming concepts including memory management, performance optimization, and real-time rendering.

### 🎮 What is Raycasting?

Raycasting is a 3D rendering technique that creates the illusion of a 3D environment by casting rays from the player's position into the scene. Each ray determines the distance to the nearest wall, which is then used to calculate the height of the wall column to be drawn on screen.

## ✨ Features

### Core Features
- **Real-time 3D rendering** using raycasting algorithm
- **Textured walls** with XPM format support
- **Smooth player movement** with collision detection
- **Mouse look controls** for immersive experience
- **Minimap display** showing player position and direction
- **Configurable floor and ceiling colors**
- **Performance monitoring** with FPS tracking
- **Cross-platform support** (macOS and Linux)

### Advanced Features
- **DDA (Digital Differential Analyzer)** algorithm for efficient ray-wall intersection
- **Smooth movement system** with acceleration and deceleration
- **Memory arena allocation** for efficient memory management
- **Robust map parsing** with comprehensive validation
- **Fallback texture system** for missing textures
- **UI elements** including weapon sprites
- **Background process support** for continuous rendering

## 🏗️ Technical Architecture

### Core Components

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   Map Parser    │    │   Raycasting    │    │    Renderer     │
│                 │    │                 │    │                 │
│ • File reading  │───▶│ • DDA Algorithm │───▶│ • 3D Projection │
│ • Validation    │    │ • Wall Detection│    │ • Texture Map   │
│ • Config setup  │    │ • Distance Calc │    │ • Pixel Drawing │
└─────────────────┘    └─────────────────┘    └─────────────────┘
         │                       │                       │
         ▼                       ▼                       ▼
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│ Player System   │    │ Movement Engine │    │  Performance    │
│                 │    │                 │    │                 │
│ • Position      │◀───│ • Input Handle  │    │ • FPS Tracking  │
│ • Direction     │    │ • Collision Det │    │ • Frame Timing  │
│ • View Angle    │    │ • Smooth Motion │    │ • Optimization  │
└─────────────────┘    └─────────────────┘    └─────────────────┘
```

### Memory Management
- **Arena Allocator**: Custom memory management system for efficient allocation/deallocation
- **Automatic Cleanup**: Centralized cleanup system preventing memory leaks
- **Resource Management**: Smart handling of textures, map data, and game objects

## 🛠️ Installation

### Prerequisites

#### For macOS:
```bash
# Xcode Command Line Tools
xcode-select --install

# MLX library (included in project)


### Building the Project

```bash
# Clone the repository
git clone [repository-url] cub3d
cd cub3d

# Build the project
make

# For debug build with additional flags
make CFLAGS="-Wall -Wextra -Werror -g"

# Clean build files
make clean

# Complete cleanup (including executable)
make fclean

# Rebuild everything
make re
```

## 🚀 Usage

### Basic Usage
```bash
# Run with default map
./cub3d map.cub

# Run with custom map
./cub3d maps/maze_test.cub

# Available test maps
./cub3d maps/simple_test.cub
./cub3d maps/complex_test.cub
./cub3d maps/subject_test.cub
```

### Command Line Arguments
- **Single argument**: Path to a `.cub` map file
- **File validation**: Automatic validation of file extension and content
- **Error handling**: Comprehensive error messages for invalid inputs

## 🎮 Controls

### Movement Controls
| Key | Action |
|-----|--------|
| `W` | Move Forward |
| `S` | Move Backward |
| `A` | Strafe Left |
| `D` | Strafe Right |
| `←` | Rotate Left |
| `→` | Rotate Right |
| `ESC` | Exit Game |
| `SPACE` | Toggle Weapon Display |

### Mouse Controls
- **Mouse Movement**: Look around (X-axis rotation)
- **Automatic Centering**: Mouse cursor automatically centers
- **Sensitivity**: Configurable mouse sensitivity (`MOUSE_SENSITIVITY = 0.001`)

### Advanced Controls
- **Smooth Movement**: Acceleration and deceleration for natural feel
- **Collision Detection**: Prevents walking through walls
- **Continuous Input**: Multiple keys can be pressed simultaneously

## 🗺️ Map Configuration

### Map File Format (.cub)

```
NO textures/north_wall.xpm
SO textures/south_wall.xpm  
WE textures/west_wall.xpm
EA textures/east_wall.xpm

F 220,100,0
C 225,30,0

111111111111
100000000001
101000000001
100000000001
100000N00001
100000000001
100000000001
111111111111
```

### Configuration Elements

#### Texture Definitions
- `NO`: North wall texture path
- `SO`: South wall texture path  
- `WE`: West wall texture path
- `EA`: East wall texture path

#### Color Definitions
- `F R,G,B`: Floor color (RGB values 0-255)
- `C R,G,B`: Ceiling color (RGB values 0-255)

#### Map Characters
- `0`: Empty space (walkable)
- `1`: Wall (solid)
- `N`: Player start position facing North
- `S`: Player start position facing South
- `E`: Player start position facing East
- `W`: Player start position facing West
- ` ` (space): Empty space outside map boundaries

### Map Validation Rules

1. **File Extension**: Must be `.cub`
2. **Required Elements**: All 6 configuration elements must be present
3. **Player Position**: Exactly one player start position
4. **Map Closure**: Map must be surrounded by walls or spaces
5. **Valid Characters**: Only allowed characters in map area
6. **Texture Files**: All texture files must exist and be valid XPM format

### Example Maps

The project includes several test maps:

- **simple_test.cub**: Basic rectangular map for testing
- **complex_test.cub**: Complex layout with multiple rooms
- **maze_test.cub**: Maze-like structure for navigation testing
- **subject_test.cub**: Map following project subject requirements

## 📁 Project Structure

```
cub3d/
├── 📁 includes/
│   ├── cub3d.h                    # Main header file
│   ├── 📁 libft/                  # Custom C library
│   │   ├── libft.h               # Library functions
│   │   ├── arena.h               # Memory arena management
│   │   ├── ft_printf.h           # Custom printf implementation
│   │   └── *.c                   # Library source files
│   ├── 📁 minilibx_opengl_20191021/  # MLX for macOS
│   └── 📁 minilibx-linux/        # MLX for Linux
├── 📁 srcs/
│   ├── main.c                    # Program entry point
│   ├── 📁 parsing/               # Map and config parsing
│   │   ├── parser.c              # Main parser logic
│   │   ├── map_parser.c          # Map structure parsing
│   │   ├── color_parser.c        # RGB color parsing
│   │   ├── texture_parser.c      # Texture path parsing
│   │   ├── map_validator.c       # Map validation
│   │   ├── position_validator.c  # Position validation
│   │   ├── player_parser.c       # Player position parsing
│   │   ├── config_parser.c       # Configuration parsing
│   │   ├── config_adapter.c      # Config to game adaptation
│   │   ├── element_parser.c      # Element parsing
│   │   ├── file_handler.c        # File I/O operations
│   │   ├── string_utils.c        # String utilities
│   │   ├── map_init.c           # Map initialization
│   │   └── parser_utils.h        # Parser headers
│   ├── 📁 raycasting/            # 3D rendering engine
│   │   ├── raycasting.c          # Main raycasting algorithm
│   │   ├── raycasting_utils.c    # Raycasting utilities
│   │   └── render.c              # 3D rendering and game loop
│   ├── 📁 player/                # Player management
│   │   ├── player.c              # Player initialization and direction
│   │   └── collision.c           # Collision detection system
│   ├── 📁 movement/              # Movement system
│   │   ├── moves.c               # Movement functions
│   │   └── keys.c                # Input handling
│   ├── 📁 draw/                  # Drawing and rendering
│   │   ├── drawing.c             # Basic drawing functions
│   │   ├── close_game.c          # Game cleanup
│   │   └── mouse.c               # Mouse input handling
│   ├── 📁 textures/              # Texture management
│   │   ├── textures.c            # Texture loading and management
│   │   └── textures_utils.c      # Texture utilities
│   └── 📁 performance/           # Performance optimization
│       ├── performance.c         # Performance tracking
│       ├── performance_utils.c   # Performance utilities
│       └── timing.c              # Timing functions
├── 📁 maps/                      # Test maps
│   ├── map.cub                   # Default map
│   ├── simple_test.cub          # Simple test map
│   ├── complex_test.cub         # Complex test map
│   ├── maze_test.cub            # Maze test map
│   └── *.cub                    # Additional test maps
├── 📁 textures/                  # Texture assets
│   ├── *.xpm                    # Wall textures
│   ├── 📁 simonkraft/           # Minecraft-style textures
│   ├── 📁 wolfenstein/          # Wolfenstein-style textures
│   └── 📁 test/                 # Test textures
├── Makefile                      # Build configuration
├── README.md                     # This file
└── Evaluation_explanation.txt    # Project explanation
```

## 🧮 Algorithm Details

### Raycasting Algorithm

The core of Cub3D is the **raycasting algorithm**, which works as follows:

#### 1. Ray Generation
```c
// For each screen column
for (int x = 0; x < WINDOW_WIDTH; x++) {
    // Calculate ray direction
    double camera_x = 2 * x / (double)WINDOW_WIDTH - 1;
    double ray_dir_x = player.dir_x + player.plane_x * camera_x;
    double ray_dir_y = player.dir_y + player.plane_y * camera_x;
}
```

#### 2. DDA Algorithm
The **Digital Differential Analyzer (DDA)** efficiently finds ray-wall intersections:

```c
// Calculate step direction and initial side distances
if (ray_dir_x < 0) {
    step_x = -1;
    side_dist_x = (player.x - map_x) * delta_dist_x;
} else {
    step_x = 1;
    side_dist_x = (map_x + 1.0 - player.x) * delta_dist_x;
}

// Perform DDA
while (hit == 0) {
    if (side_dist_x < side_dist_y) {
        side_dist_x += delta_dist_x;
        map_x += step_x;
        side = 0;
    } else {
        side_dist_y += delta_dist_y;
        map_y += step_y;
        side = 1;
    }
    if (map[map_x][map_y] == '1') hit = 1;
}
```

#### 3. Wall Height Calculation
```c
// Calculate perpendicular wall distance
if (side == 0) {
    perp_wall_dist = (map_x - player.x + (1 - step_x) / 2) / ray_dir_x;
} else {
    perp_wall_dist = (map_y - player.y + (1 - step_y) / 2) / ray_dir_y;
}

// Calculate wall height on screen
int line_height = (int)(WINDOW_HEIGHT / perp_wall_dist);
```

### Texture Mapping

#### Wall Texture Selection
```c
// Determine which texture to use based on wall side
if (side == 0) {
    texture = (step_x < 0) ? west_texture : east_texture;
} else {
    texture = (step_y < 0) ? north_texture : south_texture;
}
```

#### Texture Coordinate Calculation
```c
// Calculate texture x-coordinate
double wall_x = (side == 0) ? 
    player.y + perp_wall_dist * ray_dir_y :
    player.x + perp_wall_dist * ray_dir_x;
wall_x -= floor(wall_x);
int tex_x = (int)(wall_x * texture.width);
```

### Collision Detection

#### Basic Collision Check
```c
bool is_collision(double x, double y, t_game *game) {
    // Check corners of player bounding box
    if (collision_check(x - PLAYER_SIZE, y - PLAYER_SIZE, game)) return true;
    if (collision_check(x + PLAYER_SIZE, y - PLAYER_SIZE, game)) return true;
    if (collision_check(x - PLAYER_SIZE, y + PLAYER_SIZE, game)) return true;
    if (collision_check(x + PLAYER_SIZE, y + PLAYER_SIZE, game)) return true;
    return false;
}
```

#### Swept Collision Detection
For smooth movement, the engine uses **swept collision detection**:
```c
bool swept_collision_check(t_player *player, double new_x, double new_y, t_game *game) {
    double steps = 10.0;
    double step_x = (new_x - player->x) / steps;
    double step_y = (new_y - player->y) / steps;
    
    for (int i = 0; i < steps; i++) {
        double check_x = player->x + step_x * i;
        double check_y = player->y + step_y * i;
        if (is_collision(check_x, check_y, game)) return true;
    }
    return false;
}
```

## ⚡ Performance Features

### Frame Rate Optimization

#### FPS Tracking
```c
typedef struct s_perf {
    double  frame_time;
    double  render_time;
    double  logic_time;
    int     ray_cast;
    int     pixels_drawn;
    double  avg_fps;
    double  fps_samples[60];
    int     sample_index;
} t_perf;
```

#### Delta Time Movement
```c
void smooth_player_movement(t_player *player, double delta_time) {
    // Update velocities based on input
    player->velocity_x = lerp(player->velocity_x, target_velocity_x, 
                             acceleration * delta_time);
    player->velocity_y = lerp(player->velocity_y, target_velocity_y, 
                             acceleration * delta_time);
    
    // Apply movement with collision detection
    new_pos_with_collision_smooth_mvmt(player, delta_time);
}
```

### Memory Optimization

#### Arena Allocator
The project uses a custom **arena allocator** for efficient memory management:
```c
typedef struct s_arena {
    char    *memory;
    size_t  size;
    size_t  offset;
} t_arena;

void *arena_alloc(t_arena *arena, size_t size) {
    if (arena->offset + size > arena->size) return NULL;
    void *ptr = arena->memory + arena->offset;
    arena->offset += size;
    return ptr;
}
```

### Rendering Optimizations

#### Fast Image Clearing
```c
void fast_clear_image(t_game *game) {
    ft_memset(game->win_img.addr, 0, 
              game->win_img.line_length * game->win_img.height);
}
```

#### Efficient Pixel Drawing
```c
void put_pixel(int x, int y, int color, t_game *game) {
    if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT) {
        int pixel_index = y * game->win_img.line_length + x * (game->win_img.bpp / 8);
        *(int *)(game->win_img.addr + pixel_index) = color;
    }
}
```

## 🎨 Graphics Features

### Texture System
- **XPM Format Support**: Industry-standard texture format
- **Fallback Textures**: Automatic colored fallbacks for missing textures
- **Multiple Texture Sets**: Support for different texture themes

### Visual Effects
- **Minimap**: Real-time 2D overhead view
- **Weapon Sprites**: UI elements with weapon display
- **Smooth Rendering**: Anti-aliased movement and rotation

### Color System
- **RGB Color Support**: Full 24-bit color for floors and ceilings
- **Configurable Palette**: Easy color customization via map files

## 🔧 Configuration

### Build Configuration
The `Makefile` supports multiple build configurations:

```makefile
# Debug build
CFLAGS += -g -fsanitize=address

# Release build
CFLAGS = -Wall -Wextra -Werror -O2

# Platform-specific settings
ifeq ($(UNAME_S),Darwin)
    MLX_DIR = includes/minilibx_opengl_20191021
    FRAMEWORK = -framework OpenGL -framework AppKit
else
    MLX_DIR = includes/minilibx_linux
    FRAMEWORK = -lmlx_Linux -lXext -lX11 -lm -lz
endif
```

### Runtime Configuration
Key constants can be modified in `cub3d.h`:

```c
// Window settings
#define WINDOW_WIDTH 750
#define WINDOW_HEIGHT 600

// Movement settings
#define MOVE_SPEED 5.0
#define ROTATE_SPEED 0.05
#define MOUSE_SENSITIVITY 0.001

// Game constants
#define BLOCK_SIZE 40
#define PLAYER_SIZE 12
#define TILE_SIZE 8
```

## 🐛 Debugging

### Debug Features
- **Performance Metrics**: Real-time FPS and timing information
- **Memory Tracking**: Arena allocator provides memory usage stats
- **Error Handling**: Comprehensive error messages for common issues

### Common Issues and Solutions

#### Map Loading Issues
```bash
# Check file permissions
ls -la maps/your_map.cub

# Validate map format
head -10 maps/your_map.cub
```

#### Texture Loading Issues
```bash
# Verify texture files exist
ls -la textures/

# Check XPM format
file textures/wall.xpm
```

#### Compilation Issues
```bash
# Clean and rebuild
make fclean && make

# Check MLX installation
ls -la includes/minilibx*/
```

## 🚀 Advanced Usage

### Custom Map Creation

#### Step-by-Step Map Creation
1. **Create Configuration Section**:
   ```
   NO textures/north_wall.xpm
   SO textures/south_wall.xpm
   WE textures/west_wall.xpm
   EA textures/east_wall.xpm
   F 200,100,50
   C 100,150,200
   ```

2. **Design Map Layout**:
   ```
   111111111
   100000001
   100N00001
   100000001
   111111111
   ```

3. **Validate Map**:
   ```bash
   ./cub3d your_custom_map.cub
   ```

### Performance Tuning

#### Optimizing for Different Hardware
```c
// Adjust render quality
#define WINDOW_WIDTH 640   // Lower resolution for older hardware
#define WINDOW_HEIGHT 480

// Adjust movement sensitivity
#define MOVE_SPEED 3.0     // Slower movement for precision
#define ROTATE_SPEED 0.03  // Slower rotation for accuracy
```

## 🤝 Contributing

### Development Guidelines
1. **Code Style**: Follow 42 School's coding standards
2. **Error Handling**: Always check return values and handle errors gracefully
3. **Memory Management**: Use the arena allocator for dynamic allocations
4. **Testing**: Test with various map configurations
5. **Documentation**: Comment complex algorithms and data structures

### Adding Features
1. **New Textures**: Add XPM files to `textures/` directory
2. **New Maps**: Create `.cub` files in `maps/` directory
3. **Code Changes**: Follow the modular structure in `srcs/`

### Bug Reports
When reporting bugs, please include:
- Operating system and version
- Map file that causes the issue
- Steps to reproduce
- Expected vs. actual behavior

## 👥 Authors

**Team Members:**
- **sngantch** - Lead Developer
  - Core raycasting engine
  - Performance optimization
  - Graphics rendering system

- **hmensah-** - Systems Developer  
  - Map parsing system
  - Configuration management
  - Input validation

### Project Attribution
This project is part of the **42 School curriculum**, specifically the **cub3d** project. It demonstrates advanced C programming concepts including:
- Graphics programming with MLX
- Real-time rendering algorithms
- Memory management techniques
- Mathematical concepts in 3D graphics

## 📚 Learning Resources

### Understanding Raycasting
- [Raycasting Tutorial by Lode Vandevenne](https://lodev.org/cgtutor/raycasting.html)
- [Wolfenstein 3D Source Code](https://github.com/id-Software/wolf3d)
- [Computer Graphics: Principles and Practice](https://www.amazon.com/Computer-Graphics-Principles-Practice-3rd/dp/0321399528)

### C Programming Resources
- [The C Programming Language by K&R](https://www.amazon.com/Programming-Language-2nd-Brian-Kernighan/dp/0131103628)
- [42 School Coding Standards](https://github.com/42School/norminette)

### Graphics Programming
- [Real-Time Rendering](https://www.amazon.com/Real-Time-Rendering-Fourth-Tomas-Akenine-Moller/dp/1138627003)
- [Mathematics for 3D Game Programming](https://www.amazon.com/Mathematics-Programming-Computer-Graphics-Third/dp/1435458869)

## 📄 License

This project is part of the 42 School curriculum. Please respect the academic integrity policies of your institution when using this code.

---

**Built with ❤️ using C and MLX**

*"Understanding raycasting is understanding the fundamentals of 3D graphics programming."*

---

### Quick Start Checklist

- [ ] Clone the repository
- [ ] Install dependencies (MLX, build tools)
- [ ] Run `make` to build the project
- [ ] Test with `./cub3d map.cub`
- [ ] Create your own map file
- [ ] Experiment with different textures
- [ ] Try the various test maps
- [ ] Read the algorithm documentation
- [ ] Contribute improvements

**Happy Raycasting! 🎮✨**
