
# Eidolune

A comprehensive game engine and backend system combining a C++ game engine, Django backend API, and Unreal Engine project integration.

## Project Structure

### Core Components

- **Source/Eidolune/** - C++ game engine implementation
  - Core game logic, character system, card mechanics
  - Game engine with trigger system and effects
  - HTTP API client for backend communication
  - Deck building and validation system
  - Character classes and customization

- **Backend/** - Django REST API
  - Game data management (cards, characters, banners)
  - User account and deck management
  - Card effect bindings and keywords
  - Character relationships and customization data
  - API endpoints for game data synchronization

- **Unreal_project/Eidolune/** - Unreal Engine 5 game client
  - Game UI and visualization
  - Character blueprints and animations
  - Level prototyping and asset management
  - (!) is ignored via gitignore for its bigger size

- **Content/** - Game content data
  - Card definitions (cards.json)
  - User deck configurations
  - VFX and UI assets

## Dependencies

### C++ (CMake)
- **cpr** - HTTP client library
- **nlohmann/json** - JSON library
- **vcpkg** - Package manager

### Backend (Django)
- Django REST Framework
- Python database abstraction

## Build Instructions

### C++ Engine
```bash
cd build
cmake --build .
```

### Django Backend
```bash
cd Backend
python manage.py migrate
python manage.py runserver
```

## API Endpoints

### Banners
- `GET /api/banners/active/` - Get active banners

### User Data
- `GET /api/users/{user_id}/data/` - Get user character data and inventory

## Development

- **Visual Studio** - Built with CMake for C++ project generation
- **Python 3.x** - Django backend
- **Unreal Engine 5** - Game client

## Environment Setup

Create a `.env` file in the Backend directory for sensitive configuration (database credentials, API keys, etc.).
