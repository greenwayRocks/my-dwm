/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */

static const char *upvol[]   = { "/usr/bin/pactl", "set-sink-volume", "0", "+5%",     NULL };
static const char *downvol[] = { "/usr/bin/pactl", "set-sink-volume", "0", "-5%",     NULL };
static const char *mutevol[] = { "/usr/bin/pactl", "set-sink-mute",   "0", "toggle",  NULL };
static const unsigned int borderpx  = 4;        /* border pixel of windows */
static const unsigned int gappx     = 5;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int vertpad            = 1;       /* vertical padding of bar */
static const int sidepad            = 1;       /* horizontal padding of bar */
static const char *fonts[]          = { "Monofur Nerd Font:size=14:antialias=true:autohint=true" };
/* static const char *fonts[]          = { "monospace:size=14" }; */
/* static const char dmenufont[]       = "monospace:size=12"; */
static const char dmenufont[]       = "Monofur Nerd Font:size=16:antialias=true:autohint=true";

#include "/home/dex/.cache/wal/colors-wal-dmenu.h"

// static const char col_gray1[]       = "#222222";
// // static const char col_gray2[]       = "#444444";
// static const char col_gray3[]       = "#bbbbbb";
// static const char col_gray4[]       = "#eeeeee";
// static const char col_cyan[]        = "#005577";
// /* static const char col_cyan[]        = "#bb83f7"; purple */
// // static const char col_choco[]         = "#080808";
// static const char col_blue[]         = "#48b4e0";

/*
static const char *colors[][3]      = {
	//               fg         bg         border   
	[SchemeNorm] = { col_gray3, col_gray1, col_choco },
	[SchemeSel]  = { col_gray4, col_cyan,  col_gray2 },
};
  // [SchemeSel]  = { col_gray4, col_cyan,  col_cyan  }, 
*/

#include "/home/dex/.cache/wal/colors-wal-dwm.h"

typedef struct {
	const char *name;
	const void *cmd;
} Sp;

/* const char *spcmd1[] = {"st", "-n", "spterm", "-g", "120x34", NULL };  */
/* const char *spcmd2[] = {"st", "-n", "spfm", "-g", "144x41", "-e", "ranger", NULL }; */
/* const char *spcmd3[] = {"keepassxc", NULL }; */

const char *spcmd1[] = {"st", "-n", "spterm", "-g", "120x34", "-e", "tm", NULL };
const char *spcmd2[] = {"st", "-n", "splauncher", "-g", "120x34", "-e", "/home/mario/bin/bashCompiler", NULL };
const char *spcmd3[] = {"st", "-n", "spfm", "-g", "120x34", "-e", "ranger", NULL };
static Sp scratchpads[] = {
	/* name          cmd  144x41 */
	{"spterm",      spcmd1},
	{"splauncher",  spcmd2},
	{"spfm",   spcmd3},
};



/* tagging     *     /           */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "Firefox", NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
	{ "St",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ "Alacritty",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
	{ NULL,		  "spterm",		NULL,		SPTAG(0),		1,			 -1 },
	{ NULL,		  "splauncher",		NULL,		SPTAG(1),		1,			 -1 },
	{ NULL,		  "spfm",	NULL,		SPTAG(2),		1,			 -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *firefox[] = { "firefox", NULL };
static const char *upbright[] = { "sudo", "brillo", "-A", "10", NULL };
static const char *downbright[] = { "sudo", "brillo", "-U", "10", NULL };
static const char *screenshot[] = {"scrot", "/home/dex/pix/screens/%Y-%m-%d-%T-shot.jpg", NULL};


static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_w,      spawn,          {.v = firefox } },

	{ 0,                            XF86XK_AudioLowerVolume, spawn, {.v = downvol } },
	{ 0,                            XF86XK_AudioMute,        spawn, {.v = mutevol } },
	{ 0,                            XF86XK_AudioRaiseVolume, spawn, {.v = upvol   } },
  { 0,                            XK_Print,                spawn, {.v = screenshot } },
  
	{ MODKEY,                       XK_F10,    spawn,          {.v = downvol } },
	{ MODKEY,                       XK_F9,     spawn,          {.v = mutevol } },
	{ MODKEY,                       XK_F11,    spawn,          {.v = upvol } },

	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_s, 	   zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_x,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_f,      fullscreen,     {0} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },

	{ MODKEY|ShiftMask,  			      XK_Return, togglescratch,  {.ui = 0 } },
	{ MODKEY,            			      XK_semicolon,	     togglescratch,  {.ui = 1 } },
	{ MODKEY,            			      XK_apostrophe,	     togglescratch,  {.ui = 2 } },

	{ MODKEY,                       XF86XK_MonBrightnessUp,     spawn,          {.v = upbright } },
	{ MODKEY,                       XF86XK_MonBrightnessDown,     spawn,          {.v = downbright } },
	{ MODKEY,                       XK_F2,     spawn,          {.v = upbright } },
	{ MODKEY,                       XK_F1,     spawn,          {.v = downbright } },

	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button1,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

