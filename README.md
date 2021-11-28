# PhaseJumpPro
Utility classes for making games with Unity.

# Documentation

The documentation very limited right now. It is a work in progress here: [Link](https://coinbump.github.io/PhaseJumpPro/)

# Modules

The code contains common design patterns for games in C#:

- Behavior Trees
- Basic objects for building node graphs/simulation graphs (for city builders, sims, etc.)
- Weighted randoms (Spawn/Loot Tables + Spawner2D with spawn radius)
- Common Shaders (Stencil, Desaturate, Invert, Single Color, RGB-Recolor, etc.)
- 2D Movement paths (straight line, circle)
- Directional Velocity (2D-Kinematic)
- Emitters/Auto Emitters (firing bullets with time/count limits, Auto Emitter for shmups)
- Formations (auto populate template objects into a formation). EXAMPLE: line of hearts to show health
- Valves (for animated doors, switches, etc.)
- Interpolation curves
- Broadcast-Listener
- Command (for undo-redo)
- Timers
- ObservedValue
- Goals (for game goals (complete N tasks to continue))
- Signal-Receptor (for tutorials)
- State Machines
- Concept-Demo (for tutorials)
- Bank for Currency (freemium apps)
- TopDown2D characters and basic steering (Flip, Wander, Wander-to-Target)
- Input Schemes (Move to Mouse for Cursors, Aim at Mouse for shooters)
- Grid-Based Games (Bejeweled, Tetris, etc.)

# Special Effects
- SpriteTrail2D shows a trail of fading sprite samples behind a moving sprite (for ghost trail effect): ![alt text](https://github.com/CoinBump/PhaseJumpPro/blob/main/Documentation/Resources/Art/SpriteTrail2D.png)
- FXFade will fade from startAlpha to endAlpha, and (optionally) cull after fade out is complete
- CameraShake2D will do a basic camera shake

# Editor Scripts

I created some scripts to make life easier for 2D Game developers:
- Grid Sprite Slicer
- XML Sprite Slicer
- Apply Pixel Art settings
- Generate Animation Clips from Spritesheets

The idea is to make prototyping simple games a bit easier by having some common design patterns in place.
