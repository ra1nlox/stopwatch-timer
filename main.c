#include "raylib.h"
#include <stdio.h>
#include <time.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "styles/dark/style_dark.h"
// #include "styles/genesis/style_genesis.h"

typedef enum
{
  MINUTES = 0,
  SECONDS = 1,
} SELECTION_TYPE;

typedef enum
{
  STOPWATCH = 0,
  TIMER = 1,
} CLOCK_TYPE;

int main()
{
  InitWindow(1200, 800, "Stopwatch");
  SetWindowState(4);
  SetTargetFPS(60);
  // Depedning on the style used, nncomment the respective call. Styles were copied from raygui repository.
  GuiLoadStyleDark();
  // GuiLoadStyleGenesis();
  GuiLoadIcons("iconset.rgi", true);

  Vector2 sc_s = {GetScreenWidth(),
                  GetScreenHeight()};
  Vector2 label_dimensions = {270, 0};
  float seconds = 0;
  float minutes = 0;
  bool clock_count = false;

  float fade_timer = 0;
  bool icon_existance = false;

  SELECTION_TYPE selection_state = SECONDS;
  CLOCK_TYPE clock_type = STOPWATCH;
  bool clock_type_display = true;

  GuiSetStyle(DEFAULT, TEXT_SIZE, 128);
  GuiSetIconScale(4);

  while (!WindowShouldClose())
  {
    // Draw
    //----------------------------------------------------------------------------------

    BeginDrawing();
    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

    Vector2 center = {sc_s.x / 2.0f, sc_s.y / 2.0f - 50};

    Rectangle centered_position = {center.x - label_dimensions.x / 2,
                                   center.y - label_dimensions.y / 2,
                                   label_dimensions.x,
                                   label_dimensions.y};
    Rectangle state_icon = {center.x - 35, center.y + 100, 8, 8};
    Rectangle seconds_selection_icon = {state_icon.x + 80, state_icon.y - 175, state_icon.width, state_icon.height};
    Rectangle minutes_selection_icon = {state_icon.x - 75, state_icon.y - 175, state_icon.width, state_icon.height};
    Rectangle reset_icon = {state_icon.x, state_icon.y - 200, state_icon.width, state_icon.height};
    Rectangle clock_type_state = {state_icon.x + 3, state_icon.y - 220, 30, 30};

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
    if (IsKeyPressed(KEY_R) && !clock_count)
    {
      seconds = 0;
      minutes = 0;
      icon_existance = true;
      clock_type_display = false;
    }
    if (IsKeyPressed(KEY_S) && !clock_count)
    {
      switch (clock_type)
      {
      case STOPWATCH:
        clock_type = TIMER;
        break;

      case TIMER:
        clock_type = STOPWATCH;
        break;

      default:
        break;
      }
    }
    if (IsKeyPressed(KEY_RIGHT))
    {
      switch (selection_state)
      {
      case SECONDS:
        break;

      case MINUTES:
        selection_state = SECONDS;
        break;

      default:
        break;
      }
    }
    if (IsKeyPressed(KEY_LEFT))
    {
      switch (selection_state)
      {
      case SECONDS:
        selection_state = MINUTES;
        break;

      case MINUTES:
        break;

      default:
        break;
      }
    }
    if ((IsKeyPressed(KEY_UP) || IsKeyPressedRepeat(KEY_UP)) && !clock_count)
    {
      switch (selection_state)
      {
      case SECONDS:
        if (seconds >= 59)
        {
          break;
        }
        ++seconds;
        break;

      case MINUTES:
        if (minutes == 99)
        {
          break;
        }
        ++minutes;
        break;

      default:
        break;
      }
    }
    if ((IsKeyPressed(KEY_DOWN) || IsKeyPressedRepeat(KEY_DOWN)) && !clock_count)
    {
      switch (selection_state)
      {
      case SECONDS:
        if ((int)seconds == 0)
        {
          break;
        }
        --seconds;
        break;

      case MINUTES:
        if (minutes == 0)
        {
          break;
        }
        --minutes;
        break;

      default:
        break;
      }
    }

    switch (selection_state)
    {
    case SECONDS:
      if (!clock_count && !icon_existance)
        GuiLabel(seconds_selection_icon, GuiIconText(ICON_ARROW_DOWN, NULL));
      break;

    case MINUTES:
      if (!clock_count && !icon_existance)
        GuiLabel(minutes_selection_icon, GuiIconText(ICON_ARROW_DOWN, NULL));
      break;

    default:
      break;
    }

    if (icon_existance)
    {
      fade_timer += GetFrameTime();
      if (fade_timer < 0.3)
      {
        GuiLabel(reset_icon, GuiIconText(ICON_CLOCK, NULL));
      }
      else
      {
        icon_existance = false;
        clock_type_display = true;
        fade_timer = 0;
      }
    }

    if (clock_type_display)
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
      char buf[20];
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
      char buf[20];
      sprintf(buf, "%02d:%02d", (int)minutes, (int)seconds);
      GuiLabel(centered_position, buf);
      GuiLabel(state_icon, GuiIconText(ICON_PLAYER_PAUSE, NULL));
    }
    EndDrawing();
  }

  CloseWindow();
  return 0;
}