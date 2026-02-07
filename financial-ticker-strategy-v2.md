# ESP32 Financial Ticker: Multi-Page Implementation Strategy

## Project Overview

**What changed:** The original single-page crypto ticker is now a 5-page rotating financial dashboard.

**Pages:**

| Page | Title | Assets | API Source |
|------|-------|--------|------------|
| 1 | Cryptocurrency | BTC, ETH, HYPE, ADA | CoinGecko |
| 2 | Stock Indices | TSX, NASDAQ, S&P 500, Dow Jones | Yahoo Finance |
| 3 | Commodities | Gold, Silver, Oil, Coffee | Yahoo Finance |
| 4 | Foreign Exchange | USD/CAD, EUR/USD, USD/JPY, GBP/USD | Yahoo Finance |
| 5 | Weather | Toronto, ON | OpenWeatherMap |

**Rotation:** 1 page per minute → full cycle every 5 minutes → data refreshes each full cycle.

**Hardware:** Same as before — FireBeetle 2 ESP32-E + GDEY075T7 7.5" e-paper (800×480).

---

## Architecture: How the Rotation Works

This is the key design change. Instead of "wake → fetch → display → deep sleep for 2 min", the new flow is:

```
BOOT → Connect WiFi → Check: is this a "data refresh" cycle?
  │
  ├─ YES (every 5th wake): Fetch ALL data from all APIs, store in RTC memory
  │                         Then display the current page
  │
  └─ NO (pages 2-5 of cycle): Read cached data from RTC memory
                                Display the next page in sequence
  │
  └─ Deep sleep for 60 seconds → repeat
```

### RTC Memory is the Key

The ESP32 has **RTC (Real-Time Clock) memory** that survives deep sleep. The original weather project already uses this. We'll store:

- `currentPage` (0-4) — which page to show next
- Cached price/chart data for all 5 pages
- Timestamps for when data was last fetched

```cpp
// Stored in RTC memory (survives deep sleep)
RTC_DATA_ATTR int currentPage = 0;
RTC_DATA_ATTR unsigned long lastDataFetch = 0;
RTC_DATA_ATTR CryptoPageData cryptoCache;
RTC_DATA_ATTR IndicesPageData indicesCache;
RTC_DATA_ATTR CommoditiesPageData commoditiesCache;
RTC_DATA_ATTR ForexPageData forexCache;
RTC_DATA_ATTR WeatherPageData weatherCache;
```

### Main Loop Logic

```cpp
void setup() {
    // 1. Init display + check battery
    // 2. Connect WiFi
    // 3. Sync time via NTP

    // 4. Check if we need fresh data
    time_t now = time(nullptr);
    bool needsRefresh = (now - lastDataFetch > 300); // 5 min = 300 sec

    if (needsRefresh) {
        fetchAllData();            // Hit all APIs
        lastDataFetch = now;
    }

    // 5. Render the current page
    renderPage(currentPage);

    // 6. Advance to next page
    currentPage = (currentPage + 1) % 5;

    // 7. Deep sleep 60 seconds
    esp_deep_sleep(60 * 1000000ULL);  // microseconds
}
```

---

## API Strategy

### Why Yahoo Finance for Stocks/Commodities/FX?

Yahoo Finance's chart API is:
- **Free** with no API key required
- **One API** covers stocks, indices, commodities, AND forex
- Widely used in DIY ticker projects, so well-documented
- Returns JSON with current price + historical data in a single call

The only downside is it's an unofficial API that could change, but it's been stable for years and is the standard for hobbyist projects.

### API Endpoints

**CoinGecko (Crypto) — Page 1:**
```
GET https://api.coingecko.com/api/v3/coins/markets
    ?vs_currency=cad
    &ids=bitcoin,ethereum,hyperliquid,cardano
    &sparkline=true
    &price_change_percentage=24h,7d,30d,1y
```
Returns: prices in CAD, all % changes, and 7-day sparkline data in one call.

**Yahoo Finance (Indices) — Page 2:**
```
GET https://query1.finance.yahoo.com/v8/finance/chart/^GSPTSE?range=1mo&interval=1d
GET https://query1.finance.yahoo.com/v8/finance/chart/^IXIC?range=1mo&interval=1d
GET https://query1.finance.yahoo.com/v8/finance/chart/^GSPC?range=1mo&interval=1d
GET https://query1.finance.yahoo.com/v8/finance/chart/^DJI?range=1mo&interval=1d
```

**Yahoo Finance (Commodities) — Page 3:**
```
GET https://query1.finance.yahoo.com/v8/finance/chart/GC=F?range=1mo&interval=1d     # Gold
GET https://query1.finance.yahoo.com/v8/finance/chart/SI=F?range=1mo&interval=1d     # Silver
GET https://query1.finance.yahoo.com/v8/finance/chart/CL=F?range=1mo&interval=1d     # Crude Oil (WTI)
GET https://query1.finance.yahoo.com/v8/finance/chart/KC=F?range=1mo&interval=1d     # Coffee
```

**Yahoo Finance (Forex) — Page 4:**
```
GET https://query1.finance.yahoo.com/v8/finance/chart/CADUSD=X?range=1mo&interval=1d
GET https://query1.finance.yahoo.com/v8/finance/chart/EURUSD=X?range=1mo&interval=1d
GET https://query1.finance.yahoo.com/v8/finance/chart/JPY=X?range=1mo&interval=1d
GET https://query1.finance.yahoo.com/v8/finance/chart/GBPUSD=X?range=1mo&interval=1d
```

**OpenWeatherMap (Weather) — Page 5:**
```
GET https://api.openweathermap.org/data/2.5/weather?lat=43.65&lon=-79.38&appid=KEY&units=metric
GET https://api.openweathermap.org/data/2.5/forecast?lat=43.65&lon=-79.38&appid=KEY&units=metric
```
Already implemented in the original project — keep it.

### Yahoo Finance Response Format

Every Yahoo Finance chart call returns the same structure:
```json
{
  "chart": {
    "result": [{
      "meta": {
        "regularMarketPrice": 6025.44,
        "previousClose": 5994.71,
        "currency": "USD",
        "symbol": "^GSPC"
      },
      "timestamp": [1706745600, 1706832000, ...],
      "indicators": {
        "quote": [{
          "close": [5998.12, 6010.44, ...]
        }]
      }
    }]
  }
}
```

This gives you current price, previous close (to calculate day change), and historical closes for the sparkline chart — all in one call per symbol.

---

## Data Structures

### Generic Asset Structure

Since all 4 financial pages display the same layout (icon, name, price, changes, sparkline), use a single generic struct:

```cpp
// Generic asset data — works for crypto, stocks, commodities, and forex
struct AssetData {
    char symbol[8];           // "BTC", "^GSPC", "GC=F", "USDCAD"
    char name[24];            // "Bitcoin", "S&P 500", "Gold", "USD/CAD"
    char displaySymbol[8];    // What shows in the icon circle
    float price;              // Current price/rate
    float previousClose;      // For calculating day change
    float change_day;         // % change today
    float change_week;        // % change 7 days
    float change_month;       // % change 30 days
    float change_ytd;         // % change year-to-date (or 1y for crypto)
    float sparkline[30];      // 30 data points for chart
    int sparklineCount;       // How many points are valid
    bool valid;               // Data fetch succeeded
};

// A "page" of 4 assets
struct PageData {
    AssetData assets[4];
    time_t lastUpdated;
    bool valid;
};
```

### Complete RTC Memory Layout

```cpp
// Page rotation state
RTC_DATA_ATTR int currentPage = 0;
RTC_DATA_ATTR unsigned long lastDataFetch = 0;

// Financial data pages
RTC_DATA_ATTR PageData cryptoPage;
RTC_DATA_ATTR PageData indicesPage;
RTC_DATA_ATTR PageData commoditiesPage;
RTC_DATA_ATTR PageData forexPage;

// Weather data (keep original struct from weather project)
RTC_DATA_ATTR WeatherData weatherData;

// NOTE: RTC memory on ESP32 is 8KB. Each PageData with 4 assets 
// and 30-point sparklines is ~600 bytes. 4 pages = ~2.4KB.
// Weather data ~500 bytes. Total ~3KB — fits comfortably.
```

---

## Configuration: Easy to Customize

This is your requirement #5 — make it easy to change what's tracked. Create a single config file:

```cpp
// ============================================================
//  user_config.h — EDIT THIS FILE TO CUSTOMIZE YOUR TICKER
// ============================================================

#ifndef USER_CONFIG_H
#define USER_CONFIG_H

// ── WiFi ─────────────────────────────────────────────────────
#define WIFI_SSID       "YourNetworkName"
#define WIFI_PASSWORD   "YourPassword"

// ── API Keys ─────────────────────────────────────────────────
#define COINGECKO_API_KEY   "your-free-key"       // https://coingecko.com/en/api
#define OWM_API_KEY         "your-free-key"        // https://openweathermap.org/api

// ── Page 1: CRYPTOCURRENCY ──────────────────────────────────
// CoinGecko IDs: bitcoin, ethereum, hyperliquid, cardano,
//   solana, dogecoin, ripple, polkadot, avalanche-2, chainlink
#define CRYPTO_1_ID "bitcoin"
#define CRYPTO_1_SYMBOL "BTC"
#define CRYPTO_1_NAME "Bitcoin"

#define CRYPTO_2_ID "ethereum"
#define CRYPTO_2_SYMBOL "ETH"
#define CRYPTO_2_NAME "Ethereum"

#define CRYPTO_3_ID "hyperliquid"
#define CRYPTO_3_SYMBOL "HYPE"
#define CRYPTO_3_NAME "Hyperliquid"

#define CRYPTO_4_ID "cardano"
#define CRYPTO_4_SYMBOL "ADA"
#define CRYPTO_4_NAME "Cardano"

// ── Page 2: STOCK INDICES ───────────────────────────────────
// Yahoo Finance symbols: ^GSPTSE (TSX), ^IXIC (NASDAQ),
//   ^GSPC (S&P 500), ^DJI (Dow), ^FTSE, ^N225, ^HSI
#define INDEX_1_SYMBOL "^GSPTSE"
#define INDEX_1_DISPLAY "TSX"
#define INDEX_1_NAME "S&P/TSX"

#define INDEX_2_SYMBOL "^IXIC"
#define INDEX_2_DISPLAY "NDQ"
#define INDEX_2_NAME "NASDAQ"

#define INDEX_3_SYMBOL "^GSPC"
#define INDEX_3_DISPLAY "S&P"
#define INDEX_3_NAME "S&P 500"

#define INDEX_4_SYMBOL "^DJI"
#define INDEX_4_DISPLAY "DJIA"
#define INDEX_4_NAME "Dow Jones"

// ── Page 3: COMMODITIES ─────────────────────────────────────
// Yahoo Finance futures: GC=F (Gold), SI=F (Silver),
//   CL=F (WTI Oil), KC=F (Coffee), HG=F (Copper),
//   NG=F (Natural Gas), PL=F (Platinum), ZW=F (Wheat)
#define COMMODITY_1_SYMBOL "GC=F"
#define COMMODITY_1_DISPLAY "AU"
#define COMMODITY_1_NAME "Gold"
#define COMMODITY_1_UNIT "/oz"

#define COMMODITY_2_SYMBOL "SI=F"
#define COMMODITY_2_DISPLAY "AG"
#define COMMODITY_2_NAME "Silver"
#define COMMODITY_2_UNIT "/oz"

#define COMMODITY_3_SYMBOL "CL=F"
#define COMMODITY_3_DISPLAY "OIL"
#define COMMODITY_3_NAME "Crude Oil"
#define COMMODITY_3_UNIT "/bbl"

#define COMMODITY_4_SYMBOL "KC=F"
#define COMMODITY_4_DISPLAY "KC"
#define COMMODITY_4_NAME "Coffee"
#define COMMODITY_4_UNIT "/lb"

// ── Page 4: CURRENCIES ──────────────────────────────────────
// Yahoo Finance forex: CADUSD=X, EURUSD=X, JPY=X, GBPUSD=X,
//   AUDUSD=X, NZDUSD=X, CHFUSD=X, CNYUSD=X
#define FX_1_SYMBOL "CADUSD=X"
#define FX_1_DISPLAY "$/$"
#define FX_1_NAME "USD / CAD"

#define FX_2_SYMBOL "EURUSD=X"
#define FX_2_DISPLAY "€/$"
#define FX_2_NAME "EUR / USD"

#define FX_3_SYMBOL "JPY=X"
#define FX_3_DISPLAY "$/¥"
#define FX_3_NAME "USD / JPY"

#define FX_4_SYMBOL "GBPUSD=X"
#define FX_4_DISPLAY "£/$"
#define FX_4_NAME "GBP / USD"

// ── Page 5: WEATHER ─────────────────────────────────────────
#define WEATHER_LAT 43.6532
#define WEATHER_LON -79.3832
#define WEATHER_CITY "Toronto, ON"
#define WEATHER_UNITS "metric"   // "metric" for °C, "imperial" for °F

// ── Timing ──────────────────────────────────────────────────
#define PAGE_DURATION_SEC   60   // Seconds per page (60 = 1 min)
#define TOTAL_PAGES         5    // Number of pages in rotation
// Data refreshes every full cycle: PAGE_DURATION_SEC * TOTAL_PAGES

// ── Display ─────────────────────────────────────────────────
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 480
#define CURRENCY_PRIMARY "CAD"   // For commodity/crypto CAD conversion
#define USD_TO_CAD_FALLBACK 1.42 // Used if forex fetch fails

#endif
```

To change what's tracked, you literally just edit the symbol strings and re-upload. No code changes needed.

---

## Display Layout

All 4 financial pages use the identical 2x2 card grid layout from your existing v4 mockup. This means one `renderFinancialPage()` function handles all of them — it just takes different data and a different header title.

```
┌─────────────────────────────────────────────────────────────┐
│  ₿ CRYPTOCURRENCY                    Feb 7, 2026 · 2:47 PM │  ← Header (32px)
├────────────────────────────┬────────────────────────────────┤
│ [BTC] Bitcoin              │ [ETH] Ethereum                 │
│ C$138,245                  │ C$4,892                        │
│ US$97,420                  │ US$3,445                       │
│ 24h:+2.3% 7d:+5.1%        │ 24h:+1.8% 7d:+3.2%            │
│ 30d:+12.4% 1y:+89%        │ 30d:+8.7% 1y:+67%             │
│ ╔══════════════════╗       │ ╔══════════════════╗           │
│ ║   30d sparkline  ║       │ ║   30d sparkline  ║           │
│ ╚══════════════════╝       │ ╚══════════════════╝           │
├────────────────────────────┼────────────────────────────────┤
│ [HYPE] Hyperliquid         │ [ADA] Cardano                  │
│ C$32.84                    │ C$1.42                         │
│ US$23.12                   │ US$1.00                        │
│ 24h:+4.7% 7d:+11.3%       │ 24h:+0.8% 7d:-1.2%            │
│ 30d:+28.5% 1y:—           │ 30d:+4.5% 1y:+34%             │
│ ╔══════════════════╗       │ ╔══════════════════╗           │
│ ║   30d sparkline  ║       │ ║   30d sparkline  ║           │
│ ╚══════════════════╝       │ ╚══════════════════╝           │
├─────────────────────────────────────────────────────────────┤
│  🔋 87%  WiFi: Connected       Next: 60s · Data: 5 min     │  ← Footer (24px)
└─────────────────────────────────────────────────────────────┘
```

---

## Page 5: Weather — Zero Changes to the Original Display

The weather page is **not redesigned**. It renders the exact same output as the original esp32-weather-epd project:

- **Top bar:** City name, date, time, WiFi icon, battery percentage
- **Top-left:** Large weather icon + current temperature + description + high/low + feels like
- **Top-right:** 5-day forecast row with weather icons, temps, and precipitation probability
- **Bottom-left:** Wind, humidity, pressure, UV index, visibility, dew point, sunrise/sunset, moon phase, AQI, and indoor BME280 sensor readings (temperature, humidity, pressure)
- **Bottom-right:** 48-hour hourly outlook graph with temperature line and precipitation probability bars
- **Footer:** Last update time, data source

### What This Means for Code

The original `renderer.cpp` contains all the weather drawing functions:
- `drawCurrentConditions()` — large icon, temp, description
- `drawForecast()` — 5-day forecast row
- `drawAlerts()` — weather alerts if present
- `drawLocationDate()` — city, date, time header
- `drawOutlookGraph()` — hourly temp/precip graph
- `drawStatusBar()` — battery, WiFi, refresh info
- `drawIndoorTempHum()` — BME280 sensor display
- Plus all the weather icon bitmap drawing functions

**All of these stay in the codebase untouched.** When `currentPage == 4`, the main loop calls the original weather render pipeline. The only change is wrapping it behind the page rotation `if` statement.

This also means:
- **Keep the BME280 sensor code** — indoor temp/humidity still works
- **Keep all weather icon header files** (`wi-*.h`) — they're still rendered
- **Keep the locale strings** — weather descriptions still need them
- **Keep the OpenWeatherMap API calls** in `client_utils.cpp` — they run during the data fetch cycle alongside CoinGecko and Yahoo Finance calls
- **Keep `api_response.h` weather structs** — add the new financial structs alongside them, don't replace them

### Modified Architecture Approach

Instead of gutting `renderer.cpp` and replacing everything, the approach is now **additive**:

```cpp
// In renderer.cpp — ADD new functions, don't remove old ones

// Existing weather functions (KEEP ALL)
void drawCurrentConditions(...) { /* untouched */ }
void drawForecast(...) { /* untouched */ }
void drawOutlookGraph(...) { /* untouched */ }
void drawStatusBar(...) { /* untouched */ }
// ... all other weather rendering functions stay

// NEW: Financial page rendering
void renderFinancialPage(GxEPD2_BW &display, const PageData &data, 
                         const char* title, int pageNum) {
    // Draws the 2x2 card grid for crypto/indices/commodities/forex
}

// NEW: Main page dispatcher
void renderPage(GxEPD2_BW &display, int pageNumber) {
    switch (pageNumber) {
        case 0: renderFinancialPage(display, cryptoPage, "CRYPTOCURRENCY", 1); break;
        case 1: renderFinancialPage(display, indicesPage, "STOCK INDICES", 2); break;
        case 2: renderFinancialPage(display, commoditiesPage, "COMMODITIES", 3); break;
        case 3: renderFinancialPage(display, forexPage, "FX RATES", 4); break;
        case 4: 
            // Call the ORIGINAL weather rendering pipeline — completely untouched
            drawCurrentConditions(display, weatherData);
            drawForecast(display, forecastData);
            drawOutlookGraph(display, hourlyData);
            drawStatusBar(display, batteryVoltage, wifiStatus);
            // etc — same calls the original main.cpp makes
            break;
    }
}
```

Similarly, `api_response.h` adds the new `AssetData` / `PageData` structs **alongside** the existing weather data structs, rather than replacing them. And `client_utils.cpp` adds `fetchYahooFinance()` and `fetchCoinGecko()` functions alongside the existing `fetchWeatherData()` functions.

---

## Files to Modify (Updated)

| File | Changes | Notes |
|------|---------|-------|
| `include/user_config.h` | **NEW** | All user-editable settings in one place |
| `include/config.h` | Additions only | Add financial constants alongside existing weather ones |
| `include/api_response.h` | Additions only | Add `AssetData`/`PageData` structs, **keep all weather structs** |
| `src/api_response.cpp` | Additions only | Add CoinGecko + Yahoo Finance parsers, **keep all weather parsers** |
| `include/client_utils.h` | Additions only | Add `fetchYahooFinance()`, **keep all weather fetch functions** |
| `src/client_utils.cpp` | Additions only | Yahoo Finance HTTP client, **keep OpenWeatherMap client** |
| `include/renderer.h` | Additions only | Add `renderFinancialPage()`, `renderPage()`, **keep all weather draw functions** |
| `src/renderer.cpp` | Additions only | Financial card grid drawing, **keep entire weather rendering pipeline** |
| `src/main.cpp` | Major rewrite | Page rotation logic with RTC memory, calls weather OR financial render |
| `platformio.ini` | No changes | Keep ALL existing libraries including BME280 |

---

## Implementation Order

### Phase 1: Get the base project running (2-3 hours)
Same as before — clone, configure, verify the weather station works on your hardware. **This is your baseline. The weather display should look exactly as it does today at the end of the entire project.**

### Phase 2: Add page rotation framework (3-4 hours)
Before touching any APIs, implement the page rotation with dummy data:
- Add RTC memory variables for `currentPage`
- Create `renderFinancialPage()` that draws the 2x2 grid with hardcoded placeholder values
- Wire up `renderPage()` dispatcher: pages 0-3 call financial renderer, page 4 calls the original weather pipeline
- Verify the display cycles through 5 pages every 60 seconds
- **Confirm the weather page still renders identically** — this is your canary

### Phase 3: CoinGecko integration (4-5 hours)
Wire up real crypto data for page 1. Add the new structs and parsers alongside (not replacing) the existing weather code. Test thoroughly.

### Phase 4: Yahoo Finance integration (5-7 hours)
Build the Yahoo Finance HTTP client and JSON parser. Start with one symbol (e.g., ^GSPC for S&P 500), get it rendering correctly, then extend to all 12 symbols across pages 2-4.

### Phase 5: Verify weather page is untouched (1-2 hours)
After all the new code is added, do a careful comparison: does page 5 still render identically to the original weather station? Check every element — icons, temps, forecast, graph, indoor sensor, battery, WiFi status. If anything drifted, fix it.

### Phase 6: Polish and testing (4-6 hours)
- Error handling for each API failing independently
- CAD conversion for USD-denominated assets
- Fine-tune layout spacing
- Battery life testing over 24 hours

---

## E-Paper Refresh Considerations

Refreshing e-paper every 60 seconds is more aggressive than the original project's 15-30 minute interval. Some things to keep in mind:

**Ghosting:** E-paper displays can develop ghosting (faint remnants of previous images) with frequent partial refreshes. The GxEPD2 library supports full refresh (slower, no ghosting) and partial refresh (faster, some ghosting). Recommendation: do a full refresh every page change since you have 60 seconds — that's plenty of time.

**Display lifespan:** Modern e-paper panels are rated for millions of refresh cycles. At 1 refresh/minute, that's 525,600/year — well within spec.

**Battery impact:** Each wake cycle (WiFi connect + display refresh) draws significant power. With a 6500mAh battery:
- Original project (15 min refresh): ~months of battery life
- Your project (1 min refresh, WiFi every 5 min): estimate 1-2 weeks
- Consider: USB-C charging while plugged in may be the practical approach for a desk display

---

## Rate Limits and API Costs

| API | Free Tier Limit | Your Usage | Status |
|-----|----------------|------------|--------|
| CoinGecko | 10-30 calls/min | 1 call every 5 min | ✅ Safe |
| Yahoo Finance | No official limit (unofficial API) | 12 calls every 5 min | ✅ Safe |
| OpenWeatherMap | 1000 calls/day | 288 calls/day (every 5 min) | ✅ Safe |

---

## Quick Reference: Yahoo Finance Symbols

### Stock Indices
| Index | Symbol | Notes |
|-------|--------|-------|
| S&P/TSX Composite | `^GSPTSE` | Canadian main index |
| NASDAQ Composite | `^IXIC` | US tech-heavy |
| S&P 500 | `^GSPC` | US large cap |
| Dow Jones Industrial | `^DJI` | US blue chip |
| FTSE 100 | `^FTSE` | UK |
| Nikkei 225 | `^N225` | Japan |
| Hang Seng | `^HSI` | Hong Kong |
| DAX | `^GDAXI` | Germany |

### Commodities (Futures)
| Commodity | Symbol | Unit |
|-----------|--------|------|
| Gold | `GC=F` | per oz |
| Silver | `SI=F` | per oz |
| Crude Oil (WTI) | `CL=F` | per barrel |
| Coffee (Arabica) | `KC=F` | per lb |
| Copper | `HG=F` | per lb |
| Natural Gas | `NG=F` | per MMBtu |
| Platinum | `PL=F` | per oz |
| Wheat | `ZW=F` | per bushel |

### Forex
| Pair | Symbol |
|------|--------|
| USD/CAD | `CADUSD=X` |
| EUR/USD | `EURUSD=X` |
| USD/JPY | `JPY=X` |
| GBP/USD | `GBPUSD=X` |
| AUD/USD | `AUDUSD=X` |
| NZD/USD | `NZDUSD=X` |
| USD/CHF | `CHF=X` |

---

## Next Steps

1. Review this mockup and confirm the layout looks right for all 5 pages
2. Get the original weather station running on your hardware
3. Start Phase 2 (page rotation with dummy data) — share the main.cpp and we'll build it together
4. Work through each API integration one page at a time
