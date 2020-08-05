/* See LICENSE file for copyright and license details. */

/* This for refering to media keys / fn keys */
#include <X11/XF86keysym.h>

/* shift view taken from luke smith's build */
/* #include "shiftview.c" */
#include "vanitygaps.c"


/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 4;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
/* static const char *fonts[]          = { "Overpass Nerd Font:size=14" }; */
/* static const char *fonts[]          = { "OpenDyslexic Nerd Font Mono:size=12" }; */
static const char *fonts[]          = { "ShureTechMono Nerd Font:size=15:antialias=true" };

/* static const char dmenufont[]       = "Ubuntu:size=12"; */
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray5[]       = "#dadada";
static const char col_gray6[]       = "#dfdfdf";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_purple[]      = "#7c6180";
static const char col_purplebright[]= "#be29c2";
static const char col_bluishpurple[] = "#956fd6";
static const char col_purple_dark[] = "#493f4a";
static const char col_green_dark[]  = "#005577";
static const char col_green[]       = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray5, col_purple, col_gray2 },
	[SchemeSel]  = { col_gray6, col_purple_dark,  col_purplebright },
};

/* tagging */
static const char *tags[] = { "", "", "", "", "", "", "", "", "" };
static const char *tagsalt[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class				instance    title       tags mask     iscentered  isfloating   monitor */
	{ "Gimp",				NULL,				NULL,				0,						0,					1,					-1 },
	{		NULL,				NULL,				"neomutt",	1<<2,					0,					0,					-1 },
	{	"ncmpcpp",		NULL,				NULL,				0,						1,					1,					-1 },
	{ "firefox",		NULL,				NULL,				1 << 8,				0,					0,					-1 },
	{ "ViberPC",		NULL,				NULL,				1 << 7,				0,					0,					-1 },
	{ "Notion",		NULL,				NULL,				1 << 3,				0,					0,					-1 },
	{ "discord",		NULL,				NULL,				1 << 7,				0,					0,					-1 },
	{ "slack",		NULL,				NULL,				1 << 7,				0,					0,					-1 },
	{ "zoom",				NULL,				NULL,				1 << 4,				0,					0,					-1 },
	{ "Telegram",		NULL,				NULL,				1 << 7,				0,					0,					-1 },
	{ "Emacs",			NULL,				NULL,				1 << 2,				0,					0,					-1 },
	{ "Pavucontrol",NULL,				NULL,				1<<5,					1,					1,					-1 },
	{ "pulsemixer",NULL,				NULL,				1<<5,					1,					1,					-1 },
	{ "Zathura",		NULL,				NULL,				1 << 1,				0,					0,					-1 },
	{	"firefox",				NULL,		"Daily Plan - Evernote - Mozilla Firefox",	1<<5,		1,		1,		-1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int attachbelow = 1;    /* 1 means attach after the currently active window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
};
/* key definitions */
#define MODKEY Mod4Mask
#define ALTKEY Mod1Mask
#define CAKEYS(KEY,CMD) \
	{ ALTKEY|ControlMask,           KEY,      spawn,           {.v = CMD } },
#define CAFOCUS(KEY,CMD,TAG) \
	{ ALTKEY|ControlMask,           KEY,      spawn,           {.v = CMD } }, \
	{ ControlMask|ALTKEY,						KEY,      view,           {.ui = 1 << TAG} },
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */

static const char *dmenucmd[]		= { "rofi", "-show", "run", NULL };
static const char *searchwindow[]	= { "rofi", "-show", "window", NULL };
static const char *emacs[]			= { "emacs" , NULL};
static const char *files[]			= { "pcmanfm" , NULL};
static const char *mutt[]			= { "/home/bibek/.local/bin/runonce", "neomutt", "neomutt", NULL};
/* music */
static const char *music[]			= { "st", "-f", "ShureTechMono Nerd Font:pixelsize=20", "-c", "ncmpcpp", "-e", "ncmpcpp", NULL};
static const char *mpctoggle[]		= { "mpcc", "toggle", NULL};
static const char *mpcnext[]		= { "mpcc", "next", NULL};
static const char *mpccvup[]		= { "mpcc", "vup", NULL};
static const char *mpccvdown[]		= { "mpcc", "vdown", NULL};
static const char *mpcprev[]		= { "mpcc", "prev", NULL};
static const char *seekf[]			= { "mpc", "seek", "+5", NULL};
static const char *seekb[]			= { "mpc", "seek", "-5", NULL};
static const char *vup[]			= { "/home/bibek/.local/bin/volumecontrol", "up", NULL};
static const char *vdown[]			= { "/home/bibek/.local/bin/volumecontrol", "down", NULL};
static const char *mute[]			= { "/home/bibek/.local/bin/mute", NULL};
static const char *mutemic[]		= { "/home/bibek/.local/bin/mute", "mic", NULL};
static const char *pavucontrol[]	= { "/home/bibek/.local/bin/runonce", "pulsemixer", "pulsemixer", NULL};
/* brightness */ /*  scale by  */
static const char *bup[]			= { "light", "-T", "1.5", NULL};
static const char *bdown[]			= {  "light", "-T", "0.66", NULL};
/* utils  */
static const char *jupyter[]		= { "/home/bibek/.local/bin/jupyterlauncher", NULL };
static const char *omnidoer[]		= { "/home/bibek/.local/bin/omnidoer", NULL };
static const char *omnilauncher[]	= { "/home/bibek/.local/bin/omnilauncher", NULL };

static const char *slock[]			= { "slock", NULL };
static const char *firefox[]		= { "firefox", NULL };
static const char *termcmd[]		= { "st", NULL, NULL };

static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "60x15", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_p,      spawn, SHCMD("/usr/local/bin/omnidoer Killall") },
	/* { MODKEY|ShiftMask,            XK_Return, spawn,          {.v = termcmd } }, */
	{ MODKEY,                       XK_grave,  togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	/* { MODKEY,                  XK_minus,  removesystrayicon,   {.i = +1} }, */
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,						XK_w,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	/* { MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} }, */
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,						XK_y,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,             XK_y,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,						XK_f,      togglefullscr,  {0} },
	/* below assigns all tags */
	/* { MODKEY,                       XK_0,      view,           {.ui = ~0 } }, */
	{ MODKEY,						XK_Escape,      tag,            {.ui = ~0 } },

	/* { MODKEY,                       XK_comma,		view,       {.i = -1 } }, */
	/* { MODKEY,                       XK_period,		view,       {.i = +1 } }, */

	/* Dual monitor thing.  */
	/* { MODKEY|ShiftMask,             XK_comma,		tagmon,         {.i = -1 } }, */
	/* { MODKEY|ShiftMask,             XK_period,		tagmon,         {.i = +1 } }, */

	{ MODKEY,                       XK_n,			togglealttag,   {0} },

	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_0,                      3)
	TAGKEYS(                        XK_minus,                      4)
	TAGKEYS(                        XK_equal,                      5)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	/* { MODKEY|ShiftMask,             XK_BackSpace,      tag,   {.ui = 0} },
	 * didn't work */
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },

/* refer this for keysyms */
/* "https://www.cl.cam.ac.uk/~mgk25/ucs/keysymdef.h" */

	{ MODKEY,                       XK_slash,      spawn,      {.v = searchwindow} },

	{ MODKEY,     XK_apostrophe,		spawn,         SHCMD("/home/bibek/.local/bin/pick-emoji") },

	{ MODKEY,     XK_semicolon,				spawn,         {.v = termcmd } },
	CAKEYS(				XK_g,                      firefox)
	CAKEYS(				XK_l,                      slock)
	CAKEYS(				XK_f,                      files)
	CAKEYS(				XK_e,                      emacs)
	CAKEYS(				XK_k,                      music)
	CAKEYS(				XK_j,                      jupyter)
	CAKEYS(				XK_o,                      omnilauncher)

	/* Run a command and get focus to a window */
					/*		KEY													CMD								TAG */
	CAFOCUS(				XK_m,                      mutt,								2)
	CAFOCUS(				XK_p,                      pavucontrol,					5)

	{ MODKEY,        XK_o,			 spawn,					{.v = omnidoer } },

/* Here's a link to the keys defined in X11
 https://cgit.freedesktop.org/xorg/proto/x11proto/tree/XF86keysym.h */

	{ 0,             XF86XK_AudioPrev,			 spawn,          {.v = mpcprev } },
	{ 0,             XF86XK_AudioPlay,			 spawn,          {.v = mpctoggle } },
	{ 0,             XF86XK_AudioNext,     spawn,          {.v = mpcnext } },
	{ 0,             XF86XK_AudioLowerVolume,     spawn,          {.v = vdown} },
	{ 0,             XF86XK_AudioRaiseVolume,     spawn,          {.v = vup} },

	{ MODKEY,             XK_F8,			 spawn,          {.v = mpcprev } },
	{ MODKEY,             XK_F9,			 spawn,          {.v = mpctoggle } },
	{ MODKEY,             XK_F10,     spawn,          {.v = mpcnext } },
	{ MODKEY,             XK_F11,     spawn,          {.v = vdown} },
	{ MODKEY+ShiftMask,             XK_F11,     spawn,    SHCMD("/home/bibek/.local/bin/volumecontrol altdown") },
	{ MODKEY,             XK_F12,     spawn,          {.v = vup} },
	{ MODKEY+ShiftMask,             XK_F12,     spawn,    SHCMD("/home/bibek/.local/bin/volumecontrol altup") },

	{ 0,									XF86XK_MonBrightnessDown,      spawn,          {.v = bdown} },
	{ 0,									XF86XK_MonBrightnessUp,      spawn,          {.v = bup} },
	{ MODKEY,             XK_F3,      spawn,          {.v = bdown} },
	{ MODKEY,             XK_F4,      spawn,          {.v = bup} },

	{ MODKEY,				XK_Scroll_Lock,	spawn,		SHCMD("killall screenkey || screenkey -g '500x300+0+500' &") },
	/* {ALTKEY|ControlMask,	XK_Print,	spawn,		SHCMD("killall simplescreenrecorder || simplescreenrecorder --start-recording --start-hidden") }, */
	{MODKEY,	XK_Print,	spawn,		SHCMD("killall simplescreenrecorder || simplescreenrecorder --start-recording --start-hidden") },

	{ ShiftMask+MODKEY,             XK_F10,     spawn,          {.v = seekf } },
	{ ShiftMask+MODKEY,             XK_F8,			 spawn,         {.v = seekb } },
	{ ALTKEY+MODKEY,             XK_F11,     spawn,          {.v = mpccvdown } },
	{ ALTKEY+MODKEY,             XK_F12,			 spawn,         {.v = mpccvup } },


	{ ShiftMask,								XK_F10,				spawn,          {.v = seekf } },
	{ ShiftMask,								XK_F8,				spawn,         {.v = seekb } },

	{ ControlMask,             XK_F8,			 spawn,          {.v = mpcprev } },
	{ ControlMask,             XK_F9,			 spawn,          {.v = mpctoggle } },
	{ ControlMask,             XK_F10,     spawn,          {.v = mpcnext } },
	{ ControlMask,             XK_F11,     spawn,          {.v = vdown} },
	{ ControlMask,             XK_F12,     spawn,          {.v = vup} },
	{ 0,											XF86XK_AudioMute,		spawn, {.v = mute } },
	{ MODKEY,									XK_F7,				spawn, {.v = mutemic } },
	/* { 0,									XF86XK_AudioMute,				spawn, {.v = mutemic } }, */
	{0,				XK_Print,	spawn,		SHCMD("/home/bibek/.local/bin/screenshot full") },
	{ControlMask,	XK_Print,	spawn,		SHCMD("/home/bibek/.local/bin/screenshot junk") },
	{ALTKEY,		XK_Print,	spawn,		SHCMD("/home/bibek/.local/bin/screenshot path") },

	{ 0,						 XF86XK_Mail,		spawn, {.v =  mutt} },
	{ 0,						 XF86XK_Start,		spawn, {.v = dmenucmd} },

	{ALTKEY+ControlMask,	XK_space,	spawn,		SHCMD("/home/bibek/.local/bin/layout-keyboard") },

	/* Move floating windows in 9 standard positions  */
	/* These are keypads with numlock off */
 	{ MODKEY, 			XK_KP_End,    movetoedge,       {.v = "-1 1" } },
 	{ MODKEY, 			XK_KP_Down,   movetoedge,       {.v = "0 1" } },
 	{ MODKEY, 			XK_KP_Next,   movetoedge,       {.v = "1 1" } },
 	{ MODKEY, 			XK_KP_Left,   movetoedge,       {.v = "-1 0" } },
 	{ MODKEY, 			XK_KP_Begin,  movetoedge,       {.v = "0 0" } },
 	{ MODKEY, 			XK_KP_Right,  movetoedge,       {.v = "1 0" } },
 	{ MODKEY, 			XK_KP_Home,   movetoedge,       {.v = "-1 -1" } },
 	{ MODKEY, 			XK_KP_Up,     movetoedge,       {.v = "0 -1" } },
 	{ MODKEY, 			XK_KP_Prior,  movetoedge,       {.v = "1 -1" } },

	/* floating window resize with aspect preserved */
 	{ MODKEY|ShiftMask,             XK_j,      aspectresize,   {.i = +24} },
 	{ MODKEY|ShiftMask,             XK_k,      aspectresize,   {.i = -24} },

	/* vanity gaps */
	{ MODKEY|ALTKEY,              XK_h,      incrgaps,       {.i = +2 } },
	{ MODKEY|ALTKEY,              XK_l,      incrgaps,       {.i = -2 } },
	{ MODKEY|ALTKEY|ControlMask,  XK_h,      incrigaps,      {.i = +2 } },
	{ MODKEY|ALTKEY|ControlMask,  XK_l,      incrigaps,      {.i = -2 } },
	{ MODKEY|ALTKEY,              XK_0,      togglegaps,     {0} },
	{ MODKEY|ALTKEY|ShiftMask,    XK_0,      defaultgaps,    {0} },
	{ MODKEY|ALTKEY,              XK_y,      incrohgaps,     {.i = +1 } },
	{ MODKEY|ALTKEY,              XK_o,      incrohgaps,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_y,      incrovgaps,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_o,      incrovgaps,     {.i = -1 } },
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
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

