//-----------------------------------------------------------------------------
//   objects.h
//
//   Project: EPA SWMM5
//   Version: 5.0
//   Date:    6/19/07   (Build 5.0.010)
//            2/4/08    (Build 5.0.012)
//            3/11/08   (Build 5.0.013)
//   Author:  L. Rossman (EPA)
//            R. Dickinson (CDM)
//
//   Definitions of data structures.
//
//   Most SWMM 5 "objects" are represented as C data structures.
//
//   NOTE: the units shown next to each structure's properties are SWMM's
//         internal units and may be different than the units required
//         for the property as it appears in the input file.
//
//   NOTE: in many structure definitions, a blank line separates the set of
//         input properties from the set of computed output properties.
//-----------------------------------------------------------------------------

#include "mathexpr.h"

//-----------------------------------------
// LINKED LIST ENTRY FOR TABLES/TIME SERIES
//-----------------------------------------
struct  TableEntry
{
   double  x;
   double  y;
   struct  TableEntry* next;
};
typedef struct TableEntry TTableEntry;


//-------------------------
// CURVE/TIME SERIES OBJECT
//-------------------------
typedef struct
{
   char*         ID;              // Table/time series ID
   int           curveType;       // type of curve tabulated
   int           refersTo;        // reference to some other object            //(5.0.010 - LR)
   double        lastDate;        // last input date for time series
   double        x1, x2;          // current bracket on x-values
   double        y1, y2;          // current bracket on y-values
   TTableEntry*  firstEntry;      // first data point
   TTableEntry*  lastEntry;       // last data point
   TTableEntry*  thisEntry;       // current data point
}  TTable;


//-----------------
// RAIN GAGE OBJECT
//-----------------
typedef struct
{
   char*         ID;              // raingage name
   int           dataSource;      // data from time series or file 
   int           tSeries;         // rainfall data time series index
   char          fname[MAXFNAME+1]; // name of rainfall data file
   char          staID[MAXMSG+1]; // station number
   DateTime      startFileDate;   // starting date of data read from file
   DateTime      endFileDate;     // ending date of data read from file
   int           rainType;        // intensity, volume, cumulative
   int           rainInterval;    // recording time interval (seconds)
   int           rainUnits;       // rain depth units (US or SI)
   double        snowFactor;      // snow catch deficiency correction

   long          startFilePos;    // starting byte position in Rain file
   long          endFilePos;      // ending byte position in Rain file
   long          currentFilePos;  // current byte position in Rain file
   double        rainAccum;       // cumulative rainfall
   double        unitsFactor;     // units conversion factor (to inches or mm)
   DateTime      startDate;       // start date of current rainfall
   DateTime      endDate;         // end date of current rainfall
   DateTime      nextDate;        // next date with recorded rainfall
   double        rainfall;        // current rainfall (in/hr or mm/hr)
   double        nextRainfall;    // next rainfall (in/hr or mm/hr)
   double        reportRainfall;  // rainfall value used for reported results
   int           coGage;          // index of gage with same rain timeseries
   int           isUsed;          // TRUE if gage used by any subcatchment
   int           isCurrent;       // TRUE if gage's rainfall is current        //(5.0.012 - RD)
}  TGage;


//-------------------
// TEMPERATURE OBJECT
//-------------------
typedef struct
{
   int           dataSource;      // data from time series or file 
   int           tSeries;         // temperature data time series index
   DateTime      fileStartDate;   // starting date of data read from file
   double        elev;            // elev. of study area (ft)
   double        anglat;          // latitude (degrees)
   double        dtlong;          // longitude correction (hours)

   double        ta;              // air temperature (deg F)
   double        tmax;            // previous day's max. temp. (deg F)
   double        ea;              // saturation vapor pressure (in Hg)
   double        gamma;           // psychrometric constant
   double        tanAnglat;       // tangent of latitude angle
}  TTemp;


//-----------------
// WINDSPEED OBJECT
//-----------------
typedef struct
{
   int          type;             // monthly or file data
   double       aws[12];          // monthly avg. wind speed (mph)

   double        ws;              // wind speed (mph)
}  TWind;


//------------
// SNOW OBJECT
//------------
typedef struct
{
   double        snotmp;          // temp. dividing rain from snow (deg F)
   double        tipm;            // antecedent temp. index parameter
   double        rnm;             // ratio of neg. melt to melt coeff.
   double        adc[2][10];      // areal depletion curves

   double        season;          // snowmelt season
   double        removed;         // total snow plowed out of system (ft3)
}  TSnow;


//-------------------
// EVAPORATION OBJECT
//-------------------
typedef struct
{
    int          type;            // type of evaporation data
    int          tSeries;         // time series index
    double       monthlyEvap[12]; // monthly evaporation values
    double       panCoeff[12];    // monthly pan coeff. values

    double       rate;            // current evaporation rate (ft/sec)
}   TEvap;


//---------------------
// HORTON INFILTRATION
//---------------------
typedef struct
{
   double        fmin;            // minimum infil. rate (ft/sec)
   double        Fmax;            // maximum total infiltration (ft);
   double        decay;           // decay coeff. of infil. rate (1/sec)
   double        regen;           // regeneration coeff. of infil. rate (1/sec)

   double        tp;              // present time on infiltration curve (sec)
   double        f0;              // initial infil. rate (ft/sec)
}  THorton;


//-------------------------
// GREEN-AMPT INFILTRATION
//-------------------------
typedef struct
{
   double        S;               // avg. capillary suction (ft)
   double        Ks;              // saturated conductivity (ft/sec)
   double        IMDmax;          // max. soil moisture deficit (ft/ft)

   double        IMD;             // current soil moisture deficit
   double        F;               // current cumulative infiltration (ft)
   double        T;               // time needed to drain upper zone (sec)
   double        L;               // depth of upper soil zone (ft)
   double        FU;              // current moisture content of upper zone (ft)
   double        FUmax;           // saturated moisture content of upper zone (ft)
   char          Sat;             // saturation flag
}  TGrnAmpt;


//------------------------------
// SCS CURVE NUMBER INFILTRATION
//------------------------------
typedef struct
{
   double        Smax;            // max. infiltration capacity (ft)
   double        regen;           // infil. capacity regeneration constant (1/sec)
   double        Tmax;            // maximum inter-event time (sec)

   double        S;               // current infiltration capacity (ft)
   double        F;               // current cumulative infiltration (ft)
   double        P;               // current cumulative precipitation (ft)
   double        T;               // current inter-event time (sec)
   double        Se;              // current event infiltration capacity (ft)
   double        f;               // previous infiltration rate (ft/sec)

}  TCurveNum;


//-------------------
// AQUIFER OBJECT
//-------------------
typedef struct
{
    char*       ID;               // aquifer name
    double      porosity;         // soil porosity
    double      wiltingPoint;     // soil wilting point
    double      fieldCapacity;    // soil field capacity
    double      conductivity;     // soil hyd. conductivity (ft/sec)
    double      conductSlope;     // slope of conductivity v. moisture curve
    double      tensionSlope;     // slope of tension v. moisture curve
    double      upperEvapFrac;    // evaporation available in upper zone
    double      lowerEvapDepth;   // evap depth existing in lower zone (ft)
    double      lowerLossCoeff;   // coeff. for losses to deep GW (ft/sec)
    double      bottomElev;       // elevation of bottom of aquifer (ft)
    double      waterTableElev;   // initial water table elevation (ft)
    double      upperMoisture;    // initial moisture content of unsat. zone
}   TAquifer;


//------------------------
// GROUNDWATER OBJECT
//------------------------
typedef struct
{
    int           aquifer;        // index of associated gw aquifer 
    int           node;           // index of node receiving gw flow
    double        surfElev;       // elevation of ground surface (ft)
    double        a1, b1;         // ground water outflow coeff. & exponent
    double        a2, b2;         // surface water outflow coeff. & exponent
    double        a3;             // surf./ground water interaction coeff.
    double        fixedDepth;     // fixed surface water water depth (ft)
    double        nodeElev;       // elevation of receiving node invert (ft)
    double        theta;          // upper zone moisture content
    double        lowerDepth;     // depth of saturated zone (ft)
    double        oldFlow;        // gw outflow from previous time period (cfs)
    double        newFlow;        // gw outflow from current time period (cfs)
} TGroundwater;


//----------------
// SNOWMELT OBJECT
//----------------
// Snowmelt objects contain parameters that describe the melting
// process of snow packs on 3 different types of surfaces:
//   1 - plowable impervious area
//   2 - non-plowable impervious area
//   3 - pervious area
typedef struct
{
   char*         ID;              // snowmelt parameter set name
   double        snn;             // fraction of impervious area plowable
   double        si[3];           // snow depth for 100% cover
   double        dhmin[3];        // min. melt coeff. for each surface (ft/sec-F)
   double        dhmax[3];        // max. melt coeff. for each surface (ft/sec-F)
   double        tbase[3];        // base temp. for melting (F)
   double        fwfrac[3];       // free water capacity / snow depth
   double        wsnow[3];        // initial snow depth on each surface (ft)
   double        fwnow[3];        // initial free water in snow pack (ft)
   double        weplow;          // depth at which plowing begins (ft)
   double        sfrac[5];        // fractions moved to other areas by plowing
   int           toSubcatch;      // index of subcatch receiving plowed snow

   double        dhm[3];          // melt coeff. for each surface (ft/sec-F)
}  TSnowmelt;


//----------------
// SNOWPACK OBJECT
//----------------
// Snowpack objects describe the state of the snow melt process on each
// of 3 types of snow surfaces.
typedef struct
{
   int           snowmeltIndex;   // index of snow melt parameter set
   double        fArea[3];        // fraction of total area of each surface
   double        wsnow[3];        // depth of snow pack (ft)
   double        fw[3];           // depth of free water in snow pack (ft)
   double        coldc[3];        // cold content of snow pack
   double        ati[3];          // antecedent temperature index (deg F)
   double        sba[3];          // initial ASC of linear ADC
   double        awe[3];          // initial AWESI of linear ADC
   double        sbws[3];         // final AWESI of linear ADC
   double        imelt[3];        // immediate melt (ft)
}  TSnowpack;


//---------------
// SUBAREA OBJECT
//---------------
// An array of 3 subarea objects is associated with each subcatchment object.
// They describe the runoff process on 3 types of surfaces:
//   1 - impervious with no depression storage
//   2 - impervious with depression storage
//   3 - pervious
typedef struct
{
   int           routeTo;         // code indicating where outflow is sent
   double        fOutlet;         // fraction of outflow to outlet
   double        N;               // Manning's n
   double        fArea;           // fraction of total area
   double        dStore;          // depression storage (ft)

   double        alpha;           // overland flow factor
   double        inflow;          // inflow rate (ft/sec)
   double        runoff;          // runoff rate (ft/sec)
   double        depth;           // depth of surface runoff (ft)
}  TSubarea;


//-------------------------
// LAND AREA LANDUSE FACTOR
//-------------------------
typedef struct
{
   double        fraction;        // fraction of land area with land use
   double*       buildup;         // array of buildups for each pollutant
   DateTime      lastSwept;       // date/time of last street sweeping
}  TLandFactor;


//--------------------
// SUBCATCHMENT OBJECT
//--------------------
typedef struct
{
   char*         ID;              // subcatchment name
   char          rptFlag;         // reporting flag
   int           gage;            // raingage index
   int           outNode;         // outlet node index
   int           outSubcatch;     // outlet subcatchment index
   int           infil;           // infiltration object index
   TSubarea      subArea[3];      // sub-area data
   double        width;           // overland flow width (ft)
   double        area;            // area (ft2)
   double        fracImperv;      // fraction impervious
   double        slope;           // slope (ft/ft)
   double        curbLength;      // total curb length (ft)
   double*        initBuildup;     // initial pollutant buildup (mass/ft2)
   TLandFactor*  landFactor;      // array of land use factors
   TGroundwater* groundwater;     // associated groundwater data
   TSnowpack*    snowpack;        // associated snow pack data

   double        rainfall;        // current rainfall (ft/sec)
   double        losses;          // current infil + evap losses (ft/sec)
   double        runon;           // runon from other subcatchments (cfs)
   double        oldRunoff;       // previous runoff (cfs)
   double        newRunoff;       // current runoff (cfs)
   double        oldSnowDepth;    // previous snow depth (ft)
   double        newSnowDepth;    // current snow depth (ft)
   double*       oldQual;         // previous runoff quality (mass/L)
   double*       newQual;         // current runoff quality (mass/L)
   double*       pondedQual;      // ponded surface water quality (mass/ft3)
   double*       totalLoad;       // total washoff load (lbs or kg)
}  TSubcatch;


//-----------------------
// TIME PATTERN DATA
//-----------------------
typedef struct
{
   char*        ID;               // time pattern name
   int          type;             // time pattern type code
   int          count;            // number of factors
   double       factor[24];       // time pattern factors
}  TPattern;


//------------------------------
// DIRECT EXTERNAL INFLOW OBJECT
//------------------------------
struct ExtInflow
{
   int            param;         // pollutant index (flow = -1)
   int            type;          // CONCEN or MASS
   int            tSeries;       // index of inflow time series
   double         cFactor;       // units conversion factor for mass inflow
   double         baseline;      // constant baseline value
   double         sFactor;       // time series scaling factor
   struct ExtInflow* next;       // pointer to next inflow data object
};
typedef struct ExtInflow TExtInflow;


//-------------------------------
// DRY WEATHER FLOW INFLOW OBJECT
//-------------------------------
struct DwfInflow
{
   int            param;          // pollutant index (flow = -1)
   double         avgValue;       // average value (cfs or concen.)
   int            patterns[4];    // monthly, daily, hourly, weekend time patterns
   struct DwfInflow* next;        // pointer to next inflow data object
};
typedef struct DwfInflow TDwfInflow;


//-------------------
// RDII INFLOW OBJECT
//-------------------
typedef struct
{
   int           unitHyd;         // index of unit hydrograph
   double        area;            // area of sewershed (ft2)
}  TRdiiInflow;


//-----------------------------
// UNIT HYDROGRAPH GROUP OBJECT
//-----------------------------
typedef struct
{
   char*         ID;              // name of the unit hydrograph object
   int           rainGage;        // index of rain gage
   double        ia[12][3];       // initial abstraction parameters            //(5.0.010 - LR)
   double        r[12][3];        // fraction of rainfall becoming I&I
   long          tBase[12][3];    // time base of each UH in each month (sec)
   long          tPeak[12][3];    // time to peak of each UH in each month (sec).
}  TUnitHyd;


//-----------------
// TREATMENT OBJECT
//-----------------
typedef struct
{
    int          treatType;       // treatment equation type: REMOVAL/CONCEN
    MathExpr*    equation;        // treatment eqn. as tokenized math terms    //(5.0.010 - LR)
} TTreatment;


//------------
// NODE OBJECT
//------------
typedef struct
{
   char*         ID;              // node ID
   int           type;            // node type code
   int           subIndex;        // index of node's sub-category
   char          rptFlag;         // reporting flag
   double        invertElev;      // invert elevation (ft)
   double        initDepth;       // initial storage level (ft)
   double        fullDepth;       // dist. from invert to surface (ft)
   double        surDepth;        // added depth under surcharge (ft)
   double        pondedArea;      // area filled by ponded water (ft2)
   TExtInflow*   extInflow;       // pointer to external inflow data
   TDwfInflow*   dwfInflow;       // pointer to dry weather flow inflow data
   TRdiiInflow*  rdiiInflow;      // pointer to RDII inflow data
   TTreatment*   treatment;       // array of treatment data

   int           degree;          // number of outflow links
   char          updated;         // true if state has been updated
   double        crownElev;       // top of highest connecting conduit (ft)
   double        inflow;          // total inflow (cfs)
   double        outflow;         // total outflow (cfs)
   double        oldVolume;       // previous volume (ft3)
   double        newVolume;       // current volume (ft3)
   double        fullVolume;      // max. storage available (ft3)
   double        overflow;        // overflow rate (cfs)
   double        oldDepth;        // previous water depth (ft)
   double        newDepth;        // current water depth (ft)
   double        oldLatFlow;      // previous lateral inflow (cfs)
   double        newLatFlow;      // current lateral inflow (cfs)
   double*       oldQual;         // previous quality state
   double*       newQual;         // current quality state
   double        oldFlowInflow;   // previous flow inflow
   double        oldNetInflow;    // previous net inflow

   int           boundaryOpen;    // is the boundary open to the air (1 = yes, 0 = no)
   int           boundaryCond;    // the boundary condition type
}  TNode;


//---------------
// OUTFALL OBJECT
//---------------
typedef struct
{
   int        type;               // outfall type code
   char       hasFlapGate;        // true if contains flap gate
   double     fixedStage;         // fixed outfall stage (ft)
   int        tideCurve;          // index of tidal stage curve
   int        stageSeries;        // index of outfall stage time series
}  TOutfall;


//--------------------
// STORAGE UNIT OBJECT
//--------------------
typedef struct
{
   double      fEvap;             // fraction of evaporation realized
   double      aConst;            // surface area at zero height (ft2)
   double      aCoeff;            // coeff. of area v. height curve
   double      aExpon;            // exponent of area v. height curve
   int         aCurve;            // index of tabulated area v. height curve

   double      hrt;               // hydraulic residence time (sec)
}  TStorage;


//--------------------
// RATING UNIT OBJECT
//--------------------
typedef struct
{
   int         qCurve;            // index of tabulated area v. height curve
}  TRatingUnit;


//--------------------
// GATE OBJECT (WAS FLOW DIVIDER)
//--------------------
typedef struct
{
   int         link;              // index of link with diverted flow
   int         type;              // divider type code
   double      qMin;              // minimum inflow for diversion (cfs)
   double      qMax;              // flow when weir is full (cfs)
   double      dhMax;             // height of weir (ft)
   double      cWeir;             // weir discharge coeff.
   int         lossCurve;         // index of gate head loss curve
   int         tsCurve;           // index of gate open/close timeseries
   double      curOpening;        // current opening, as a percentage
}  TDivider;


//-----------------------------
// CROSS SECTION DATA STRUCTURE
//-----------------------------
typedef struct
{
   int           type;            // type code of cross section shape
   int           transect;        // index of transect/shape (if applicable)   //(5.0.010 - LR)
   double        yFull;           // depth when full (ft)
   double        wMax;            // width at widest point (ft)
   double        aFull;           // area when full (ft2)
   double        rFull;           // hyd. radius when full (ft)
   double        sFull;           // section factor when full (ft^4/3)
   double        sMax;            // section factor at max. flow (ft^4/3)

   // These variables have different meanings depending on section shape
   double        yBot;            // depth of bottom section
   double        aBot;            // area of bottom section
   double        sBot;            // slope of bottom section
   double        rBot;            // radius of bottom section
}  TXsect;


//--------------------------------------
// CROSS SECTION TRANSECT DATA STRUCTURE
//--------------------------------------
#define  N_TRANSECT_TBL  51       // size of transect geometry tables          //(5.0.010 - LR)
typedef struct
{
    char*        ID;                        // section ID
    double       yFull;                     // depth when full (ft)
    double       aFull;                     // area when full (ft2)
    double       rFull;                     // hyd. radius when full (ft)
    double       wMax;                      // width at widest point (ft)
    double       sMax;                      // section factor at max. flow (ft^4/3)
    double       aMax;                      // area at max. flow (ft2)

    double       roughness;                 // Manning's n
    double       areaTbl[N_TRANSECT_TBL];   // table of area v. depth
    double       hradTbl[N_TRANSECT_TBL];   // table of hyd. radius v. depth
    double       widthTbl[N_TRANSECT_TBL];  // table of top width v. depth
    int          nTbl;                      // size of geometry tables
}   TTransect;


//-------------------------------------                                        //(5.0.010 - LR)
// CUSTOM CROSS SECTION SHAPE STRUCTURE                                        //(5.0.010 - LR)
//-------------------------------------                                        //(5.0.010 - LR)
#define N_SHAPE_TBL  51           // size of shape geometry tables             //(5.0.010 - LR)
typedef struct                                                                 //(5.0.010 - LR)
{                                                                              //(5.0.010 - LR)
    int          curve;                     // index of shape curve            //(5.0.010 - LR) 
    int          nTbl;                      // size of geometry tables         //(5.0.010 - LR)
    double       aFull;                     // area when full                  //(5.0.010 - LR)
    double       rFull;                     // hyd. radius when full           //(5.0.010 - LR)
    double       wMax;                      // max. width                      //(5.0.010 - LR)
    double       sMax;                      // max. section factor             //(5.0.010 - LR)
    double       aMax;                      // area at max. section factor     //(5.0.010 - LR)
    double       areaTbl[N_SHAPE_TBL];      // table of area v. depth          //(5.0.010 - LR)
    double       hradTbl[N_SHAPE_TBL];      // table of hyd. radius v. depth   //(5.0.010 - LR)
    double       widthTbl[N_SHAPE_TBL];     // table of top width v. depth     //(5.0.010 - LR)
}   TShape;                                                                    //(5.0.010 - LR)


//------------
// LINK OBJECT
//------------
typedef struct
{
   char*         ID;              // link ID
   int           type;            // link type code
   int           subIndex;        // index of link's sub-category
   char          rptFlag;         // reporting flag
   int           node1;           // start node index
   int           node2;           // end node index
   double        offset1;         // ht. above start node invert (ft)          //(5.0.012 - LR)
   double        offset2;         // ht. above end node invert (ft)            //(5.0.012 - LR)
   TXsect        xsect;           // cross section data
   double        q0;              // initial flow (cfs)
   double        qLimit;          // constraint on max. flow (cfs)
   double        cLossInlet;      // inlet loss coeff.
   double        cLossOutlet;     // outlet loss coeff.
   double        cLossAvg;        // avg. loss coeff.
   int           hasFlapGate;     // true if flap gate present

   double        oldFlow;         // previous flow rate (cfs)
   double        newFlow;         // current flow rate (cfs)
   double        oldDepth;        // previous flow depth (ft)
   double        newDepth;        // current flow depth (ft)
   double        oldVolume;       // previous flow volume (ft3)
   double        newVolume;       // current flow volume (ft3)
   double        qFull;           // flow when full (cfs)
   double        setting;         // current control setting                   //(5.0.010 - LR)
   double        targetSetting;   // target control setting                    //(5.0.010 - LR)
   double        froude;          // Froude number
   double*       oldQual;         // previous quality state
   double*       newQual;         // current quality state
   int           flowClass;       // flow classification
   double        dqdh;            // change in flow w.r.t. head (ft2/sec)
   signed char   direction;       // flow direction flag
   char          isClosed;        // flap gate closed flag
////BEGIN ITM CODE
   int           initDepthCode;   // initial depth code (critical, normal, constant)
   double        initDepth;       // initial depth
   double        velocity;        // velocity
////END ITM CODE
}  TLink;


//---------------
// CONDUIT OBJECT
//---------------
typedef struct
{
   double        length;          // conduit length (ft)
   double        roughness;       // Manning's n
   char          barrels;         // number of barrels

   double        modLength;       // modified conduit length (ft)
   double        roughFactor;     // roughness factor for DW routing
   double        slope;           // slope
   double        beta;            // discharge factor
   double        qMax;            // max. flow (cfs)
   double        a1, a2;          // upstream & downstream areas (ft2)
   double        q1, q2;          // upstream & downstream flows per barrel (cfs)
   double        q1Old, q2Old;    // previous values of q1 & q2 (cfs)
// double        aMid;            // average flow area (ft2)                   //(5.0.013 - LR)
   char          capacityLimited; // capacity limited flag                     //(5.0.012 - LR)
   char          superCritical;   // super-critical flow flag
   char          hasLosses;       // local losses flag
}  TConduit;


//------------
// PUMP OBJECT
//------------
typedef struct
{
   int           type;            // pump type
   int           pumpCurve;       // pump curve table index
   double        initSetting;     // initial speed setting                     //(5.0.010 - LR)
   double        yOn;             // startup depth (ft)                        //(5.0.010 - LR)
   double        yOff;            // shutoff depth (ft)                        //(5.0.010 - LR)
   double        xMin;            // minimum pt. on pump curve                 //(5.0.012 - LR)
   double        xMax;            // maximum pt. on pump curve                 //(5.0.012 - LR)
}  TPump;


//---------------
// ORIFICE OBJECT
//---------------
typedef struct
{
   int           type;            // orifice type code
   int           shape;           // orifice shape code
   double        cDisch;          // discharge coeff.
   double        orate;           // time to open/close (sec)

   double        cOrif;           // coeff. for orifice flow (ft^2.5/sec)      //(5.0.012 - LR)
   double        hCrit;           // inlet depth where weir flow begins (ft)   //(5.0.012 - LR)
   double        cWeir;           // coeff. for weir flow (cfs)                //(5.0.012 - LR)
   double        length;          // equivalent length (ft)
   double        surfArea;        // equivalent surface area (ft2)
}  TOrifice;


//------------
// WEIR OBJECT
//------------
typedef struct
{
   int           type;            // weir type code
   int           shape;           // weir shape code
   //double        crestHt;         // crest height above node invert (ft)     //(5.0.012 - LR)
   double        cDisch1;         // discharge coeff.
   double        cDisch2;         // discharge coeff. for ends
   double        endCon;          // end contractions

   double        cSurcharge;      // cDisch for equiv. orifice under surcharge
   double        length;          // equivalent length (ft)
   double        slope;           // slope for Vnotch & Trapezoidal weirs
   double        surfArea;        // equivalent surface area (ft2)
}  TWeir;


//---------------------
// OUTLET DEVICE OBJECT
//---------------------
typedef struct
{
    //double       crestHt;         // crest ht. above node invert (ft)        //(5.0.012 - LR)
    double       qCoeff;          // discharge coeff.
    double       qExpon;          // discharge exponent
    int          qCurve;          // index of discharge rating curve
}   TOutlet;


//-----------------
// POLLUTANT OBJECT
//-----------------
typedef struct
{
   char*         ID;              // Pollutant ID
   int           units;           // units
   double        mcf;             // mass conversion factor
   double        pptConcen;       // precip. concen.
   double        gwConcen;        // groundwater concen.
   double        rdiiConcen;      // RDII concen.
   double        kDecay;          // decay constant (1/sec)
   int           coPollut;        // co-pollutant index
   double        coFraction;      // co-pollutant fraction
   int           snowOnly;        // TRUE if buildup occurs only under snow
}  TPollut;


//------------------------
// BUILDUP FUNCTION OBJECT
//------------------------
typedef struct
{
   int           normalizer;      // normalizer code (area or curb length)
   int           funcType;        // buildup function type code
   double        coeff[3];        // coeffs. of buildup function
   double        maxDays;         // time to reach max. buildup (days)
}  TBuildup;


//------------------------
// WASHOFF FUNCTION OBJECT
//------------------------
typedef struct
{
   int           funcType;        // washoff function type code
   double        coeff;           // function coeff.
   double        expon;           // function exponent
   double        sweepEffic;      // street sweeping fractional removal
   double        bmpEffic;        // best mgt. practice fractional removal
}  TWashoff;


//---------------
// LANDUSE OBJECT
//---------------
typedef struct
{
   char*         ID;              // landuse name
   double        sweepInterval;   // street sweeping interval (days)
   double        sweepRemoval;    // fraction of buildup available for sweeping
   double        sweepDays0;      // days since last sweeping at start
   TBuildup*     buildupFunc;     // array of buildup functions for pollutants
   TWashoff*     washoffFunc;     // array of washoff functions for pollutants
}  TLanduse;


//--------------------------
// REPORTING FLAGS STRUCTURE
//--------------------------
typedef struct
{
   char          report;          // TRUE if results report generated
   char          input;           // TRUE if input summary included
   char          subcatchments;   // TRUE if subcatchment results reported
   char          nodes;           // TRUE if node results reported
   char          links;           // TRUE if link results reported
   char          continuity;      // TRUE if continuity errors reported
   char          flowStats;       // TRUE if routing link flow stats. reported
   char          nodeStats;       // TRUE if routing node depth stats. reported
   char          controls;        // TRUE if control actions reported
   int           linesPerPage;    // number of lines printed per page
}  TRptFlags;


//-------------------------------
// CUMULATIVE RUNOFF TOTALS
//-------------------------------
typedef struct
{                                 // All volume totals are in ft.
   double        rainfall;        // rainfall volume 
   double        evap;            // evaporation loss
   double        infil;           // infiltration loss
   double        runoff;          // runoff volume
   double        initStorage;     // inital surface storage
   double        finalStorage;    // final surface storage
   double        initSnowCover;   // initial snow cover
   double        finalSnowCover;  // final snow cover
   double        snowRemoved;     // snow removal
   double        pctError;        // continuity error (%)
}  TRunoffTotals;


//--------------------------
// CUMULATIVE LOADING TOTALS
//--------------------------
typedef struct
{                                 // All loading totals are in lbs.
   double        initLoad;        // initial loading
   double        buildup;         // loading added from buildup
   double        deposition;      // loading added from wet deposition
   double        sweeping;        // loading removed by street sweeping
   double        bmpRemoval;      // loading removed by BMPs
   double        infil;           // loading removed by infiltration
   double        runoff;          // loading removed by runoff
   double        finalLoad;       // final loading
   double        pctError;        // continuity error (%)
}  TLoadingTotals;


//------------------------------
// CUMULATIVE GROUNDWATER TOTALS
//------------------------------
typedef struct
{                                 // All GW flux totals are in feet.
   double        infil;           // surface infiltration
   double        upperEvap;       // upper zone evaporation loss
   double        lowerEvap;       // lower zone evaporation loss
   double        lowerPerc;       // percolation out of lower zone
   double        gwater;          // groundwater flow
   double        initStorage;     // initial groundwater storage
   double        finalStorage;    // final groundwater storage
   double        pctError;        // continuity error (%)
}  TGwaterTotals;


//----------------------------
// CUMULATIVE ROUTING TOTALS
//----------------------------
typedef struct
{                                  // All routing totals are in ft3.
   double        dwInflow;         // dry weather inflow
   double        wwInflow;         // wet weather inflow
   double        gwInflow;         // groundwater inflow
   double        iiInflow;         // RDII inflow
   double        exInflow;         // direct inflow
   double        flooding;         // internal flooding
   double        outflow;          // external outflow
   double        reacted;          // reaction losses
   double        initStorage;      // initial storage volume
   double        finalStorage;     // final storage volume
   double        pctError;         // continuity error
}  TRoutingTotals;


//-----------------------
// SYSTEM-WIDE STATISTICS
//-----------------------
typedef struct
{
   double        minTimeStep;
   double        maxTimeStep;
   double        avgTimeStep;
   double        avgStepCount;
   double        steadyStateCount;
}  TSysStats;


//--------------------
// RAINFALL STATISTICS
//--------------------
typedef struct
{
   DateTime    startDate;
   DateTime    endDate;
   long        periodsRain;
   long        periodsMissing;
   long        periodsMalfunc;
}  TRainStats;


//------------------------
// SUBCATCHMENT STATISTICS
//------------------------
typedef struct
{
    double       precip;
    double       runon;
    double       evap;
    double       infil;
    double       runoff;
    double       maxFlow;         
}  TSubcatchStats;


//----------------
// NODE STATISTICS
//----------------
typedef struct
{
   double        avgDepth;
   double        maxDepth;
   DateTime      maxDepthDate;
   double        maxDepthChange;                                               //(5.0.012 - LR)
   double        volFlooded;
   double        timeFlooded;
   double        timeSurcharged;                                               //(5.0.012 - LR)
   double        timeCourantCritical;
   double        totLatFlow;                                                   //(5.0.012 - LR)
   double        maxLatFlow;
   double        maxInflow;
   double        maxOverflow;
   double        maxPondedVol;                                                 //(5.0.012 - LR)
   DateTime      maxInflowDate;
   DateTime      maxOverflowDate;
}  TNodeStats;


//-------------------
// STORAGE STATISTICS
//-------------------
typedef struct
{
   double        avgVol;
   double        maxVol;
   double        maxFlow;
   DateTime      maxVolDate;
}  TStorageStats;


//-------------------
// OUTFALL STATISTICS
//-------------------
typedef struct
{
   double       avgFlow;
   double       maxFlow;
   double*      totalLoad;   
   int          totalPeriods;
}  TOutfallStats;


//----------------                                                             //(5.0.012 - LR)
// PUMP STATISTICS                                                             //(5.0.012 - LR)
//----------------                                                             //(5.0.012 - LR)
typedef struct                                                                 //(5.0.012 - LR)
{                                                                              //(5.0.012 - LR)
   double       utilized;                                                      //(5.0.012 - LR)
   double       avgFlow;                                                       //(5.0.012 - LR)
   double       maxFlow;                                                       //(5.0.012 - LR)
   double       volume;                                                        //(5.0.012 - LR)
   double       energy;                                                        //(5.0.012 - LR)
   double       offCurve;                                                      //(5.0.012 - LR)
   int          totalPeriods;                                                  //(5.0.012 - LR)
}  TPumpStats;


//----------------
// LINK STATISTICS
//----------------
typedef struct
{
   double        maxFlow;
   DateTime      maxFlowDate;
   double        maxVeloc;
   DateTime      maxVelocDate;
   double        maxDepth;
   double        avgFlowChange;
   double        avgFroude;
   double        timeSurcharged;
   double        timeFullUpstream;                                             //(5.0.012 - LR)
   double        timeFullDnstream;                                             //(5.0.012 - LR)
   double        timeFullFlow;                                                 //(5.0.012 - LR)
   double        timeCapacityLimited;                                          //(5.0.012 - LR)
   double        timeInFlowClass[MAX_FLOW_CLASSES];
   double        timeCourantCritical;
   long          flowTurns;                                                    //(5.0.010 - LR)
   int           flowTurnSign;                                                 //(5.0.010 - LR)
}  TLinkStats;


//-------------------------
// MAXIMUM VALUE STATISTICS
//-------------------------
typedef struct
{
   int           objType;         // either NODE or LINK
   int           index;           // node or link index
   double        value;           // value of node or link statistic
}  TMaxStats; 


//------------------
// REPORT FIELD INFO
//------------------
typedef struct 
{
   char          Name[80];        // name of reported variable 
   char          Units[80];       // units of reported variable
   char          Enabled;         // TRUE if appears in report table
   int           Precision;       // number of decimal places when reported
}  TRptField;


//-----------------
// FILE INFORMATION
//-----------------
typedef struct
{
   char          name[MAXFNAME+1];     // file name
   char          mode;                 // NONE, SCRATCH, USE, or SAVE
   char          state;                // current state (OPENED, CLOSED)
   FILE*         file;                 // FILE structure pointer
}  TFile;