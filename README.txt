Emscripten port of "Circus Linux!". I am NOT the original author of the game, only the port. 
Part of the port code was written by LLM. There might be some bugs, but it seems playable.

ORIGINAL README BELOW:

README.txt for Circus Linux!

by Bill Kendrick
bill@newbreedsoftware.com
http://www.newbreedsoftware.com/circus-linux/

Version 1.0.3

April 28, 2001


DESCRIPTION
-----------
  "Circus Linux!" is based on the Atari 2600 game "Circus Atari" by Atari,
  released in 1980.  Gameplay is similar to "Breakout" and "Arkanoid"- you
  slide a device left and right to bounce objects into the air which destroy
  a wall.


STORY
-----
  The clowns are trying to pop balloons to score points!


DOCUMENTATION
-------------
  Important documentation for "Circus Linux!" is contained in multiple files.
  Please see them:

    AUTHORS.txt    - Credits for who made this game.
    CHANGES.txt    - Changes since the previous versions of "Circus Linux!"
    COPYING.txt    - The GNU Public License, which "Circus Linux!" is under.
    INSTALL.txt    - Instructions on requirements, compiling and installing.
    README.txt     - (This file.)  Game story, usage, rules and controls.
    FAQ.txt        - A list of Frequently Asked Questions and their answers.
    TODO.txt       - A list of things left to do to make this game perfect!


RUNNING THE GAME
----------------
  LINUX
  -----
    Double-click the "circuslinux" icon in the directory in which it is
    contained ("/usr/local/bin" by default - see INSTALL.txt and run
    "./configure --help" for details.)

    Or, run the command "circuslinux" from a terminal under X-Window.

  WINDOWS
  -------
    Double-click the "circuslinux.exe" icon in the "circus-linux" folder.

    Or, run the command "circuslinux.exe" from the Windows' Start Menu's
    "Run" command.

  OPTIONS
  -------
    The program accepts some options:

      --disable-sound     - If sound support was compiled in, this will
                            disable it for this session of the game.

      --fullscreen        - Run in fullscreen mode.

      --low               - Disable some features so that the game might run
                            better on low-end hardware.  Titlescreen ballons,
                            the game background, and circus acts are disabled,
                            and the game screen refreshes every 1/15th of a
                            second instead of 1/30th of a second.

      --digital           - When using a joystick, left and right cause the
                            teeter-totter to move incrementally.  (This is
                            as opposed to the default mode, where the analog
                            position of the joystick is used literally.)
                            Useful for playing with a digital gamepad.

      --paddle            - For using real Atari 2600 paddles connected to
                            a your PC's joystick port.

      --help              - Display a help message summarizing command-line
                            options, copyright, and game controls.

      --usage             - Display a brief message summarizing command-line
                            options.

      --version           - Display the version of Circus Linux! you're
                            running.


TITLE SCREEN
------------
  At the title screen, you can use the mouse to do a number of things.

  Choose to begin a game:

    ONE PLAYER
      One player popping balloons.

    TWO PLAYER
      Two players take turns popping their own set of balloons.

    TWO PLAYERS COOPERATIVE
      Two players take turns popping the same set of balloons.


  Set some game mode options to make the game more difficult:

    BARRIERS
      A set of deflective barriers float just below the balloons.

    BOUNCY BALLOONS
      When a clown hits a balloon, he's deflected back down.

    CLEAR ALL
      All three rows of balloons must be cleared before any balloons
      come back.

    Note: There are eight high scores in Circus Linux!  One for every
    combination of the above options.  For example, the high score you
    get playing in BARRIERS mode is different than the high score for
    CLEAR ALL mode.


  Set sound volumes:

    SOUND EFFECTS
    MUSIC


  View the high scores:

    HIGH SCORES


  And quit the game:

    EXIT


  Note: You can also press the [ESCAPE] key to quit the game from the
  title screen.

  Note: If you're playing Circus Linux! in a window, you can also use
  your window manager's close option (usually an "X" button on the window's
  title bar) to quit.  This works at ANY time.


SCREEN LAYOUT
-------------
    +---------------------------+
    |243       1467         :)x4|    Score, Number of Clowns
    |* * * * * * * *     *     *|
    | * * *         * *         |    Balloons
    |        *       * * *     *|
    |     ---    ---    ---     |    Barriers
    |           X               |    Flying Clown
    |                           |
    |     X_-                   |
    |]    -o                   [|    Rubber bouncers and teeter-totter
    +---------------------------+

  Status
  ------
    At the top of the screen you see the current player's score,
    which player is currently playing, and how many clowns they
    have left.

    Note: PLAYER 1's score is displayed in RED.
          PLAYER 2's score is displayed in BLUE.

  Ballons
  -------
    Below that is three rows of colorful balloons.  The top and
    bottom rows are moving one direction, and the middle is moving
    the opposite direction.

  Barriers
  --------
    If the BARRIERS option was set on the title screen, a set of three
    deflective barriers appear under the three rows of balloons.

  Bouncers
  --------
    At the bottom of the screen there are two pink rubber bouncers,
    one at the far left, and one at the far right.

  Teeter-Totter
  -------------
    Also at the bottom of the screen is a teeter-totter with a clown
    on it.  This is what you control.


PLAYING THE GAME
----------------
  Lives
  -----
    You begin the game with five clowns.  When you run out of clowns, your
    game is over.

  Launching a Clown
  -----------------
    To launch a clown:
      Mouse:     Click a mouse button
      Keyboard:  Press RETURN, ALT or SHIFT
      Joystick:  Press a fire button
      Paddle:    Press the fire button

    A clown will appear on either the far left or far right of the screen,
    above one of the two pink rubber bouncers.  He'll bounce one the bouncer
    once, and then head towards the middle of the screen.

  Catching a Clown
  ----------------
    To move the teeter-totter left and right:
      Mouse:     Move the mouse left and right
      Keyboard:  Press the LEFT or RIGHT arrow keys
      Joystick:  Use the joystick's stick or directional pad

    Try to catch the flying clown with the teeter-totter.  If he lands on
    the high end of the teeter-totter, the clown on the low end will be
    launched into the air to, hopefully, pop some balloons.

    Note: The closer to the edge a clown lands, the higher up the other
    clown is thrown into the air.

  Missing a Clown
  ---------------
    If a clown hits the ground, or hits the low end of the teeter-totter,
    he'll splat into the ground and will be out of the game.  (The number
    of clowns listed at the top right of the screen will decrease by one.)

  Controlling the Teeter-Totter
  -----------------------------
    To flip the teeter-totter:
      Mouse:     Click a mouse button
      Keyboard:  Press RETURN, ALT or SHIFT
      Joystick:  Press a fire button
      Paddle:    Press the fire button

    This causes the low and high ends of the teeter-totter to change
    places, useful when the teeter-totter is at a far side of the screen
    and the low end is on the outside.

  Grabbing the Mouse
  ------------------
    If you're playing in windowed-mode (ie, not full-screen mode), and you
    wish the mouse pointer to be 'trapped' within the Circus Linux! window,
    you can do so by turning "Mouse Grabbing" on.

    To do this, press the [G] key.  The mouse will no longer be able to
    go outside of the game window.  (When you exit the game and see the
    final score screen, the mouse will be released.)

    To turn mouse grabbing back off, just press the [G] key again.

  Pausing
  -------
    To pause the game, you can press either the [SPACE], [TAB] or [P]
    key.  To unpause, press one of those keys again.

  Quitting
  --------
    To quit the current game, you can press [ESCAPE].  This forfeits your game
    (as though you ran out of lives).

    To quit Circus Linux! altogether, you can use your window manager's
    close command.  See the note in the "Title Screen" section, above.


SCORING
-------
  Bouncing
  --------
    Each time a clown bounces on a bouncer or the teeter-totter, you
    receive one (1) point.

  Balloons
  --------
    Balloons are worth different points, depend on which row they're
    in:

      Bottom Row:  1 point
      Middle Row:  2 points
      Top Row:     3 points

  Clearing Balloons:
  ------------------
    When a row of balloons is cleared, the entire row is filled up
    with new balloons and you receive bonus points:

      Bottom Row:  20 points
      Middle Row:  50 points
      Top Row:     100 points  and  1 extra clown

    Note: If "CLEAR ALL" mode was set on the title screen, the new
    balloons don't appear, and you don't receive any bonus points or
    extra clowns, until all three rows have been cleared.

  High Score
  ----------
    For every game mode combination, there is a high score.

      No special modes
      BARRIERS
      BOUNCY BALLOONS
      CLEAR ALL
      BARRIERS and BOUNCY BALLOONS
      BARRIERS and CLEAR ALL
      BOUNCY BALLOONS and CLEAR ALL
      All special modes: BARRIERS, BOUNCY BALLOONS and CLEAR ALL

    If you exceed the high score for the combination of modes that you're
    playing, you'll hear a "yahoo!" sound and the balloons will flash
    colors for a few moments.

    When the game ends, if you have the high score, you get to enter
    your initials.  See "Game Over Screen," below.


GAME OVER SCREEN
----------------
  When the game ends (both players ran out of lives, or you pressed
  [ESCAPE]), a screen showing the final score(s) is displayed.

  If a player has a high score, they are asked to enter their initials.
  Press [ENTER] when you're done to return to the title screen.

  If noone has a high score, simply press any key or click a mouse button
  to return to the title screen.


OPTIONS FILE
------------
  When Circus Linux! starts up, it looks for an options file.

  Under Linux, this file is ".circuslinux" in your home directory ($HOME).
  Under Windows and MacOS, this file is "circuslinux.dat" in
  Circus Linux!'s folder.

  This file records high score scores and initials, and volume settings.


THE END
-------
  Thanks for trying out Circus Linux!  Feel free to share it with your
  friends!

  If you like Circus Linux!, please rate and review it at the
  Linux Game Tome:

    http://www.happypenguin.org/show?Circus%20Linux%21


  Thanks!

  Bill Kendrick  <bill@newbreedsoftware.com>
