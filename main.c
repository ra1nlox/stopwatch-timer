#include "raylib.h"
#include <stdio.h>
#include <time.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "styles/dark/style_dark.h"
// #include "styles/genesis/style_genesis.h"

#define FADE_TIMEOUT 0.3

#define switch_clock_mode(clock_mode) \
  do                                  \
  {                                   \
    switch (clock_type)               \
    {                                 \
    case STOPWATCH:                   \
      clock_type = TIMER;             \
      break;                          \
    case TIMER:                       \
      clock_type = STOPWATCH;         \
      break;                          \
    default:                          \
      break;                          \
    }                                 \
  } while (0)

typedef enum
{
  STOPWATCH = 0,
  TIMER = 1,
} CLOCK_TYPE;

void increment_time(float *seconds, float *minutes)
{

  return;
}

int main()
{
  InitWindow(1200, 800, "Stopwatch");
  SetWindowState(FLAG_WINDOW_RESIZABLE);
  SetTargetFPS(60);
  // Depedning on the style used, nncomment the respective call. Styles were copied from raygui repository.
  GuiLoadStyleDark();
  // GuiLoadStyleGenesis();
  GuiLoadIcons("iconset.rgi", true);

  Vector2 sc_s = {GetScreenWidth(),
                  GetScreenHeight()};

  // The main
  Vector2 label_dimensions = {270, 0};
  float seconds = 0;
  float minutes = 0;
  bool clock_count = false;

  float fade_timer = 0;
  bool reset_icon_state = false;

  bool is_seconds_selected = true;
  CLOCK_TYPE clock_type = STOPWATCH;

  GuiSetStyle(DEFAULT, TEXT_SIZE, 128);
  GuiSetIconScale(4);

  while (!WindowShouldClose())
  {
    // Draw
    //----------------------------------------------------------------------------------

    BeginDrawing();
    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

    // The center of the window (not really but stylistically)
    Vector2 center = {sc_s.x / 2.0f, sc_s.y / 2.0f - 50};

    // Position for the main label (the "00:00" one)
    Rectangle centered_position = {center.x - label_dimensions.x / 2,
                                   center.y - label_dimensions.y / 2,
                                   label_dimensions.x,
                                   label_dimensions.y};
    // "State" here means play/pause
    Rectangle state_icon = {center.x - 35, center.y + 100, 8, 8};

    // The little arrows above the seconds or minutes
    Rectangle seconds_selection_icon = {state_icon.x + 80, state_icon.y - 175, state_icon.width, state_icon.height};
    Rectangle minutes_selection_icon = {state_icon.x - 75, state_icon.y - 175, state_icon.width, state_icon.height};

    // Reset is the one that appears when the R is pressed
    Rectangle reset_icon = {state_icon.x, state_icon.y - 200, state_icon.width, state_icon.height};
    Rectangle clock_type_state = {state_icon.x + 3, state_icon.y - 220, 30, 30};

    // Kinda responsive layout, since every Rectangle used is tied one way or another to other element's Rectangle
    if (IsWindowResized())
    {
      sc_s.x = GetScreenWidth();
      sc_s.y = GetScreenHeight();
      centered_position = (Rectangle){sc_s.x / 2 - label_dimensions.x / 2,
                                      sc_s.y / 2 - label_dimensions.y / 2,
                                      label_dimensions.x,
                                      label_dimensions.y};
    }

    if (IsKeyPressed(KEY_SPACE))
    {
      clock_count = !clock_count;
    }
    if (IsKeyPressed(KEY_F))
    {
      if (IsWindowMaximized())
      {
        RestoreWindow();
        printf("RESTORED");
      }
      else
      {
        MaximizeWindow();
        printf("MAXIMIZED");
      }
    }
    // The clock has to be stopped to be able to reset
    if (IsKeyPressed(KEY_R) && !clock_count)
    {
      seconds = 0;
      minutes = 0;
      reset_icon_state = true;
    }
    // The clock has to be stopped to be able to switch modes
    if (IsKeyPressed(KEY_S) && !clock_count)
    {
      switch_clock_mode(clock_type);
    }
    if (IsKeyPressed(KEY_RIGHT))
    {
      is_seconds_selected = true;
    }
    if (IsKeyPressed(KEY_LEFT))
    {
      is_seconds_selected = false;
    }
    if ((IsKeyPressed(KEY_UP) || IsKeyPressedRepeat(KEY_UP)) && !clock_count)
    {
      if (is_seconds_selected)
      {
        if (seconds < 59)
        {
          seconds = (int)seconds + 1;
        }
      }
      else
      {

        if (minutes < 99)
        {
          ++minutes;
        }
      }
    }

    if ((IsKeyPressed(KEY_DOWN) || IsKeyPressedRepeat(KEY_DOWN)) && !clock_count)
    {
      if (is_seconds_selected)
      {
        if ((int)seconds != 0)
        {
          seconds = (int)seconds - 1;
        }
      }
      else
      {
        if (minutes != 0)
        {
          --minutes;
        }
      }
    }

    // Display selection arrows
    if (is_seconds_selected)
    {
      if (!clock_count && !reset_icon_state)
      {
        GuiLabel(seconds_selection_icon, GuiIconText(ICON_ARROW_DOWN, NULL));
      }
    }
    else
    {
      if (!clock_count && !reset_icon_state)
        GuiLabel(minutes_selection_icon, GuiIconText(ICON_ARROW_DOWN, NULL));
    }

    // Small part to make icon appear and dissapear
    if (reset_icon_state)
    {
      fade_timer += GetFrameTime();
      // Increment timer and see if it is under specified value. Default is 0.3, so the icon will be displayed until fade_timer reaches 0.3 (so for 0.3 seconds)
      if (fade_timer < FADE_TIMEOUT)
      {
        GuiLabel(reset_icon, GuiIconText(ICON_CLOCK, NULL));
      }
      else
      {
        reset_icon_state = false;
        // Reset timer for future calls
        fade_timer = 0;
      }
    }

    // Normally the mode type is displayed when reset icon is not showed
    if (!reset_icon_state)
    {
      switch (clock_type)
      {
      case STOPWATCH:
        GuiLabel(clock_type_state, "S");
        break;

      case TIMER:
        GuiLabel(clock_type_state, "T");
        break;

      default:
        break;
      }
    }

    if (clock_count)
    {
      char buf[8];
      switch (clock_type)
      {
      case STOPWATCH:
        seconds += GetFrameTime();
        if (seconds > 60.0f && minutes < 100)
        {
          seconds = 0.0f;
          ++minutes;
        }
        if (minutes == 100)
        {
          clock_count = false;
          minutes = 99;
          seconds = 59;
        }

        sprintf(buf, "%02d:%02d", (int)minutes, (int)seconds);
        GuiLabel(centered_position, buf);
        GuiLabel(state_icon, GuiIconText(ICON_PLAYER_PLAY, NULL));
        break;

      case TIMER:
        if (minutes == 0 && seconds <= 0)
        {
          seconds = 0;
          clock_count = false;
        }
        seconds -= GetFrameTime();
        if (seconds <= 0.0f && minutes != 0)
        {
          seconds = 60.0f;
          --minutes;
        }
        sprintf(buf, "%02d:%02d", (int)minutes, (int)seconds);
        GuiLabel(centered_position, buf);
        GuiLabel(state_icon, GuiIconText(ICON_PLAYER_PLAY, NULL));
        break;

      default:
        break;
      }
    }
    else
    {
      char buf[8];
      sprintf(buf, "%02d:%02d", (int)minutes, (int)seconds);
      GuiLabel(centered_position, buf);
      GuiLabel(state_icon, GuiIconText(ICON_PLAYER_PAUSE, NULL));
    }
    EndDrawing();
  }

  CloseWindow();
  return 0;
}