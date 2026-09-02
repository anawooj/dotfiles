/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 2;      /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
/*defaults= 20, 10, 10, 30 */
static const unsigned int gappih    = 9;        /* horiz inner gap between windows */
static const unsigned int gappiv    = 9;        /* vert inner gap between windows */
static const unsigned int gappoh    = 9;        /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 9;        /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */

static const char *fonts[]          = { "JetBrainsMonoNerdFont-Light:size=9", "fontawesome:size=9" };
static const char dmenufont[]       = "JetBrainsMonoNerdFont-Light:size=9";

static const char col_gray1[]       = "#000000";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#4d6a8e";

/* TokyoNight colors */
static const char col_bg[]     = "#1a1b26";  // background
static const char col_fg[]     = "#a9b1d6";  // foreground
static const char col_blk[]    = "#32344a";  // black (normal)
static const char col_red[]    = "#f7768e";  // red
static const char col_grn[]    = "#9ece6a";  // green
static const char col_ylw[]    = "#e0af68";  // yellow
static const char col_blu[]    = "#7aa2f7";  // blue
static const char col_mag[]    = "#ad8ee6";  // magenta
static const char col_cyn[]    = "#0db9d7";  // cyan (highlight)
static const char col_brblk[]  = "#444b6a";  // bright black

static const char *colors[][3] = {
    /*               fg       bg       border */
    [SchemeNorm] = { col_fg,  col_bg,   col_bg },
    [SchemeSel]  = { col_mag, col_bg,   col_mag   },
};

/* tagging */
static const char *tags[] = { " ", " ", " 󰙯", "4", "5", "6", "7", "8" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class            instance    title       tags mask     isfloating   monitor */
	{ "Gimp",            NULL,       NULL,       0,            1,           -1 },
	{ "librewolf",       NULL,       NULL,       1 << 1,       0,           -1 },
	{ "webcord",         NULL,       NULL,       1 << 2,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; 	 /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    	 /* number of clients in master area */
static const int resizehints = 1;      	 /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; 	 /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1   		 /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ " 󰝘 ",        tile },    	/* first entry is default */
	{ "  ",        NULL },    	/* no layout function means floating behavior */
	{ " [M] ",      monocle },
 	{ "  ",        spiral },
 	{ " [\\] ",     dwindle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG)                                                                                               \
       &((Keychord){1, {{MODKEY, KEY}},                                        view,           {.ui = 1 << TAG} }), \
       &((Keychord){1, {{MODKEY|ControlMask, KEY}},                            toggleview,     {.ui = 1 << TAG} }), \
       &((Keychord){1, {{MODKEY|ShiftMask, KEY}},                              tag,            {.ui = 1 << TAG} }), \
       &((Keychord){1, {{MODKEY|ControlMask|ShiftMask, KEY}},                  toggletag,      {.ui = 1 << TAG} }),

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_bg, "-nf", col_gray2, "-sb", col_mag, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *screenshotcmd[] = { "escrotum", "-s",  NULL };
static const char *brighter[] = { "brightnessctl", "set", "10%+", NULL };
static const char *dimmer[]   = { "brightnessctl", "set", "10%-", NULL };
static const char *up_vol[]   = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "+2%",   NULL };
static const char *down_vol[] = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "-2%",   NULL };
static const char *mute_vol[] = { "pactl", "set-sink-mute",   "@DEFAULT_SINK@", "toggle", NULL };

static Keychord *keychords[] = {
    /* key count, modifier/key sequence,            function,        argument */

    &((Keychord){1, {{MODKEY, XK_r}},               spawn,          {.v = dmenucmd } }),
    &((Keychord){1, {{MODKEY, XK_space}},           spawn,          {.v = termcmd } }),
    &((Keychord){1, {{MODKEY, XK_Print}},           spawn,          {.v = screenshotcmd } }),

    &((Keychord){1, {{0, XF86XK_MonBrightnessDown}},spawn,	    {.v = dimmer } }),
    &((Keychord){1, {{0, XF86XK_MonBrightnessUp}},  spawn, 	    {.v = brighter } }),

    &((Keychord){1, {{0, XF86XK_AudioMute}},        spawn,          {.v = mute_vol } }),
    &((Keychord){1, {{0, XF86XK_AudioLowerVolume}}, spawn,          {.v = down_vol } }),
    &((Keychord){1, {{0, XF86XK_AudioRaiseVolume}}, spawn,          {.v = up_vol } }),

    &((Keychord){1, {{MODKEY, XK_b}},               togglebar,      {0} }),
    &((Keychord){1, {{MODKEY, XK_j}},               focusstack,     {.i = +1 } }),
    &((Keychord){1, {{MODKEY, XK_k}},               focusstack,     {.i = -1 } }),
    &((Keychord){1, {{MODKEY, XK_i}},               incnmaster,     {.i = +1 } }),
    &((Keychord){1, {{MODKEY, XK_p}},               incnmaster,     {.i = -1 } }),
    &((Keychord){1, {{MODKEY, XK_g}},               setmfact,       {.f = -0.05} }),
    &((Keychord){1, {{MODKEY, XK_h}},               setmfact,       {.f = +0.05} }),

    &((Keychord){1, {{MODKEY, XK_z}},               incrgaps,       {.i = +3 } }),
    &((Keychord){1, {{MODKEY, XK_x}},               incrgaps,       {.i = -3 } }),
    &((Keychord){1, {{MODKEY, XK_a}},               togglegaps,     {0} }),
    &((Keychord){1, {{MODKEY|ShiftMask, XK_a}},     defaultgaps,    {0} }),

    &((Keychord){1, {{MODKEY, XK_Tab}},             view,           {0} }),
    &((Keychord){1, {{MODKEY, XK_q}},               killclient,     {0} }),

    &((Keychord){1, {{MODKEY|ShiftMask, XK_t}},     setlayout,      {.v = &layouts[0]} }),
    &((Keychord){1, {{MODKEY, XK_n}},               setlayout,      {.v = &layouts[2]} }),
    &((Keychord){1, {{MODKEY, XK_c}},               setlayout,      {.v = &layouts[3]} }),
    &((Keychord){1, {{MODKEY, XK_o}},               setlayout,      {.v = &layouts[4]} }),
    &((Keychord){1, {{MODKEY|ShiftMask, XK_Return}},setlayout,      {0} }),

    &((Keychord){1, {{MODKEY|ShiftMask, XK_f}},     fullscreen,     {0} }),
    &((Keychord){1, {{MODKEY|ShiftMask, XK_space}}, togglefloating, {0} }),

    &((Keychord){1, {{MODKEY, XK_0}},               view,           {.ui = ~0 } }),
    &((Keychord){1, {{MODKEY|ShiftMask, XK_0}},     tag,            {.ui = ~0 } }),

    &((Keychord){1, {{MODKEY, XK_comma}},           focusmon,       {.i = -1 } }),
    &((Keychord){1, {{MODKEY, XK_period}},          focusmon,       {.i = +1 } }),
    &((Keychord){1, {{MODKEY|ShiftMask, XK_comma}}, tagmon,         {.i = -1 } }),
    &((Keychord){1, {{MODKEY|ShiftMask, XK_period}},tagmon,         {.i = +1 } }),

    // TAGKEYS
    TAGKEYS(                        XK_1,                      0)
    TAGKEYS(                        XK_2,                      1)
    TAGKEYS(                        XK_3,                      2)
    TAGKEYS(                        XK_4,                      3)
    TAGKEYS(                        XK_5,                      4)
    TAGKEYS(                        XK_6,                      5)
    TAGKEYS(                        XK_7,                      6)
    TAGKEYS(                        XK_8,                      7)
    TAGKEYS(                        XK_9,                      8)

    &((Keychord){1, {{MODKEY|ShiftMask,   XK_q}},   quit,           {0} }),
    &((Keychord){1, {{MODKEY|ControlMask, XK_r}},   quit,           {1} }),
};


/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

