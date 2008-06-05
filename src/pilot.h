/*
 * See Licensing and Copyright notice in naev.h
 */




#ifndef PILOT_H
#  define PILOT_H


#include "physics.h"
#include "ship.h"
#include "ai.h"
#include "outfit.h"
#include "faction.h"
#include "sound.h"
#include "economy.h"


#define PLAYER_ID       1


#define HYPERSPACE_ENGINE_DELAY  3000 /* time to warm up engine */
#define HYPERSPACE_FLY_DELAY     5000 /* time it takes to hyperspace */
#define HYPERSPACE_STARS_BLUR    2000 /* how long the stars blur */
#define HYPERSPACE_STARS_LENGTH  1000 /* length the stars blur to at max */
#define HYPERSPACE_FADEOUT       1000 /* how long the fade is */
#define HYPERSPACE_FUEL          100  /* how much fuel it takes */


#define PILOT_SIZE_APROX      0.8   /* aproximation for pilot size */
#define PILOT_DISABLED_ARMOR  0.3   /* armour % that gets it disabled */

/* hooks */
#define PILOT_HOOK_NONE    0 /* no hook */
#define PILOT_HOOK_DEATH   1 /* pilot died */
#define PILOT_HOOK_BOARD   2 /* pilot got boarded */
#define PILOT_HOOK_DISABLE 3 /* pilot got disabled */


/* flags */
#define pilot_isFlag(p,f)  ((p)->flags & (f))
#define pilot_setFlag(p,f) ((p)->flags |= (f))
#define pilot_rmFlag(p,f)  ((p)->flags ^= (f))
/* creation */
#define PILOT_PLAYER       (1<<0) /* pilot is a player */
#define PILOT_HASTURRET    (1<<20) /* pilot has turrets */
#define PILOT_NO_OUTFITS   (1<<21) /* do not create the pilot with outfits */
#define PILOT_EMPTY        (1<<22) /* do not add pilot to stack */
/* dynamic */
#define PILOT_HOSTILE      (1<<1) /* pilot is hostile to the player */
#define PILOT_COMBAT       (1<<2) /* pilot is engaged in combat */
#define PILOT_AFTERBURNER  (1<<3) /* pilot has his afterburner activated */
#define PILOT_HYP_PREP     (1<<5) /* pilot is getting ready for hyperspace */
#define PILOT_HYP_BEGIN    (1<<6) /* pilot is starting engines */
#define PILOT_HYPERSPACE   (1<<7) /* pilot is in hyperspace */
#define PILOT_BOARDED      (1<<8) /* pilot has been boarded already */
#define PILOT_DISABLED     (1<<9) /* pilot is disabled */
#define PILOT_DEAD         (1<<10) /* pilot is in it's dying throes */
#define PILOT_EXPLODED     (1<<11) /* pilot did final death explosion */
#define PILOT_DELETE       (1<<15) /* pilot will get deleted asap */

/* makes life easier */
#define pilot_isPlayer(p)  ((p)->flags & PILOT_PLAYER)
#define pilot_isDisabled(p) ((p)->flags & PILOT_DISABLED)


typedef struct PilotOutfit_ {
   Outfit* outfit; /* associated outfit */
   int quantity; /* number of outfits of this type pilot has */

   unsigned int timer; /* used to store when it was last used */
} PilotOutfit;



/*
 * pilot commodity
 */
typedef struct PilotCommodity_ {
   Commodity* commodity;
   int quantity;
   unsigned int id; /* special mission id for cargo */
} PilotCommodity;


/*
 * primary pilot structure
 */
typedef struct Pilot_ {

   unsigned int id; /* pilot's id, used for many functions */
   char* name; /* pilot's name (if unique) */
   char* title; /* title - usually indicating special properties - TODO use */

   int faction;

   /* object caracteristics */
   Ship* ship; /* ship pilot is flying */
   Solid* solid; /* associated solid (physics) */
   int tsx, tsy; /* current sprite, calculated on update */

   /* movement */
   double thrust, turn, speed;

   /* current health */
   double armour, shield, energy, fuel;
   double armour_max, shield_max, energy_max, fuel_max;
   double armour_regen, shield_regen, energy_regen;

   /* associated functions */
   void (*think)(struct Pilot_*); /* AI thinking for the pilot */
   void (*update)(struct Pilot_*, const double); /* updates the pilot */
   void (*render)(struct Pilot_*); /* for rendering the pilot */

   /* outfit management */
   PilotOutfit* outfits;
   int noutfits;
   PilotOutfit* secondary; /* secondary weapon */
   PilotOutfit* ammo; /* secondary ammo if needed */
   PilotOutfit* afterburner; /* the afterburner */

   /* cargo */
   int credits; /* monies the pilot has */
   PilotCommodity* commodities; /* commodity and quantity */
   int ncommodities;
   int cargo_free;

   /* misc */
   uint32_t flags; /* used for AI and others */
   unsigned int ptimer; /* generic timer for internal pilot use */
   int lockons; /* Stores how many seeking weapons are targetting pilot */

   /* hook attached to the pilot */
   int hook_type;
   int hook;

   /* AI */
   AI_Profile* ai; /* ai personality profile */
   unsigned int tcontrol; /* timer for control tick */
   unsigned int timer[MAX_AI_TIMERS]; /* timers for AI */
   Task* task; /* current action */
} Pilot;


/*    
 * fleets
 */   
typedef struct FleetPilot_ {
   Ship* ship; /* ship the pilot is flying */
   char* name; /* used if they have a special name like uniques */
   int chance; /* chance of this pilot appearing in the leet */
} FleetPilot;
typedef struct Fleet_ {
   char* name; /* fleet name, used as the identifier */
   int faction; /* faction of the fleet */

   AI_Profile* ai; /* AI profile to use */

   FleetPilot* pilots; /* the pilots in the fleet */
   int npilots; /* total number of pilots */
} Fleet;


/*
 * getting pilot stuff
 */
extern Pilot* player; /* the player */
Pilot* pilot_get( const unsigned int id );
unsigned int pilot_getNext( const unsigned int id );
unsigned int pilot_getNearest( const Pilot* p );
unsigned int pilot_getHostile (void); /* only for the player */
Fleet* fleet_get( const char* name );
int pilot_getJumps( const Pilot* p );


/*
 * misc
 */
void pilot_shoot( Pilot* p, const unsigned int target, const int secondary );
void pilot_hit( Pilot* p, const Solid* w, const unsigned int shooter,
      const DamageType dtype, const double damage );
void pilot_setSecondary( Pilot* p, const char* secondary );
void pilot_setAmmo( Pilot* p );
void pilot_setAfterburner( Pilot* p );
double pilot_face( Pilot* p, const float dir );
/* outfits */
int pilot_freeSpace( Pilot* p ); /* weapon space */
int pilot_addOutfit( Pilot* pilot, Outfit* outfit, int quantity );
int pilot_rmOutfit( Pilot* pilot, Outfit* outfit, int quantity );
char* pilot_getOutfits( Pilot* pilot );
void pilot_calcStats( Pilot* pilot );
/* normal cargo */
int pilot_cargoUsed( Pilot* pilot ); /* gets how much cargo it has onboard */
int pilot_cargoFree( Pilot* p ); /* cargo space */
int pilot_addCargo( Pilot* pilot, Commodity* cargo, int quantity );
int pilot_rmCargo( Pilot* pilot, Commodity* cargo, int quantity );
/* mission cargo - not to be confused with normal cargo */
unsigned int pilot_addMissionCargo( Pilot* pilot, Commodity* cargo, int quantity );
int pilot_rmMissionCargo( Pilot* pilot, unsigned int cargo_id );


/*
 * creation
 */
void pilot_init( Pilot* dest, Ship* ship, char* name, int faction, AI_Profile* ai,
      const double dir, const Vector2d* pos, const Vector2d* vel, const int flags );
unsigned int pilot_create( Ship* ship, char* name, int faction, AI_Profile* ai,
      const double dir, const Vector2d* pos, const Vector2d* vel, const int flags );
Pilot* pilot_createEmpty( Ship* ship, char* name,
      int faction, AI_Profile* ai, const int flags );
Pilot* pilot_copy( Pilot* src );


/*
 * init/cleanup
 */
void pilot_destroy(Pilot* p);
void pilots_free (void);
void pilots_clean (void);
void pilots_cleanAll (void);
void pilot_free( Pilot* p );
int fleet_load (void); /* TODO make this fleet stuff actually matter */
void fleet_free (void);


/*
 * update
 */
void pilots_update( double dt );
void pilots_render (void);


/*
 * hooks
 */
void pilot_addHook( Pilot *pilot, int type, int hook );


#endif /* PILOT_H */
