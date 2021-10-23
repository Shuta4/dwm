/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int focusonwheel       = 0;
static const char *fonts[]          = { "monospace:size=12" };
static const char dmenufont[]       = "monospace:size=12";
static const char col_norm_fg[]     = "#ffffff";
static const char col_norm_bg[]     = "#121212";
static const char col_norm_br[]     = "#121212";
static const char col_sel_fg[]      = "#ffffff";
static const char col_sel_bg[]      = "#673AB7";
static const char col_sel_br[]      = "#673AB7";
static const char *colors[][3]      = {
	/*               fg           bg           border   */
	[SchemeNorm] = { col_norm_fg, col_norm_bg, col_norm_br },
	[SchemeSel]  = { col_sel_fg,  col_sel_bg,  col_sel_br  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	/* Examples:
        { "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
        */
        { NULL,       NULL,       NULL,       0,            0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#include "fibonacci.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ KeyPress, MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ KeyPress, MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ KeyPress, MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ KeyPress, MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {
	"dmenu_run",
	"-m", dmenumon,
	"-fn", dmenufont,
	"-nb", col_norm_bg,
	"-nf", col_norm_fg,
	"-sb", col_sel_bg,
	"-sf", col_sel_fg,
	"-i",
	NULL
};
static const char *termcmd[]  = { "st", NULL };

#include <X11/XF86keysym.h>

static Key keys[] = {
	/* type       modifier                      key        function        argument */
	/* programs execution */
	{ KeyPress,   MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ KeyPress,   MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	/* Add "kill -<signal of block + 34> $(pidof dwmblocks)" to SHCMD command to update block */
	{ KeyPress,   0, XF86XK_MonBrightnessDown,             spawn,          SHCMD("brightnessctl set 5%-") },
	{ KeyPress,   0, XF86XK_MonBrightnessUp,               spawn,          SHCMD("brightnessctl set +5%") },
	{ KeyPress,   0, XF86XK_AudioMute,                     spawn,          SHCMD("pactl set-sink-mute @DEFAULT_SINK@ toggle; kill -36 $(pidof dwmblocks)") },
	{ KeyPress,   0, XF86XK_AudioLowerVolume,              spawn,          SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -5%; kill -36 $(pidof dwmblocks)") },
	{ KeyPress,   0, XF86XK_AudioRaiseVolume,              spawn,          SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +5%; kill -36 $(pidof dwmblocks)") },
	{ KeyPress,   0, XF86XK_AudioMicMute,                  spawn,          SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle") },
	{ KeyPress,   0, XF86XK_WLAN,                          spawn,          SHCMD("") },
	{ KeyPress,   0, XF86XK_Tools,                         spawn,          SHCMD("") },
	{ KeyPress,   0,                            XK_Print,  spawn,          SHCMD("flameshot gui") },
	/* windows controls */
	{ KeyPress,   MODKEY|ControlMask,           XK_b,      togglebar,      {0} },
	{ KeyPress,   MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ KeyPress,   MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ KeyPress,   MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ KeyPress,   MODKEY|ShiftMask,             XK_i,      incnmaster,     {.i = -1 } },
	{ KeyPress,   MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ KeyPress,   MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ KeyPress,   MODKEY,                       XK_Return, zoom,           {0} },
	{ KeyPress,   MODKEY,                       XK_Tab,    view,           {0} },
	/* layouts switching */
	{ KeyPress,   MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ KeyPress,   MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ KeyPress,   MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ KeyPress,   MODKEY,                       XK_r,      setlayout,      {.v = &layouts[3]} },
	{ KeyPress,   MODKEY|ShiftMask,             XK_r,      setlayout,      {.v = &layouts[4]} },
	{ KeyPress,   MODKEY,                       XK_space,  setlayout,      {0} },
	{ KeyPress,   MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	/* tags controls */
	{ KeyPress,   MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ KeyPress,   MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ KeyPress,   MODKEY,                       XK_Left,   focusmon,       {.i = -1 } },
	{ KeyPress,   MODKEY,                       XK_Right,  focusmon,       {.i = +1 } },
	{ KeyPress,   MODKEY|ShiftMask,             XK_Left,   tagmon,         {.i = -1 } },
	{ KeyPress,   MODKEY|ShiftMask,             XK_Right,  tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	/* exit */
	{ KeyPress,   MODKEY,                       XK_q,      killclient,     {0} },
	{ KeyPress,   MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

