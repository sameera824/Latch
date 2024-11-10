/**
 * SYNTAX:
 *
 * RealtimeDatabase::set<T>(<AsyncClient>, <path>, <value>, <AsyncResultCallback>, <uid>);
 *
 * T - The type of value to set.
 * <AsyncClient> - The async client.
 * <path> - The node path to set the value.
 * <value> - The value to set.
 * <AsyncResultCallback> - The async result callback (AsyncResultCallback).
 * <uid> - The user specified UID of async result (optional).
 *
 * The complete usage guidelines, please visit https://github.com/mobizt/FirebaseClient
 */

#include <Arduino.h>
#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W) || defined(ARDUINO_GIGA) || defined(ARDUINO_OPTA)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#elif __has_include(<WiFiNINA.h>) || defined(ARDUINO_NANO_RP2040_CONNECT)
#include <WiFiNINA.h>
#elif __has_include(<WiFi101.h>)
#include <WiFi101.h>
#elif __has_include(<WiFiS3.h>) || defined(ARDUINO_UNOWIFIR4)
#include <WiFiS3.h>
#elif __has_include(<WiFiC3.h>) || defined(ARDUINO_PORTENTA_C33)
#include <WiFiC3.h>
#elif __has_include(<WiFi.h>)
#include <WiFi.h>
#endif

#include <FirebaseClient.h>

// #define WIFI_SSID "PPT wifi"
// #define WIFI_PASSWORD "tP6DFYy?Vg*&X%3}"

#define WIFI_SSID "Dialog Home"
#define WIFI_PASSWORD "Work@home"

// The API key can be obtained from Firebase console > Project Overview > Project settings.
#define API_KEY "AIzaSyAA7sn8fyBh3LBH4Y7U5JYWepRmZGpoY8s"

// User Email and password that already registerd or added in your project.
#define USER_EMAIL "sameera824@gmail.com"
#define USER_PASSWORD "Sameera@777"
#define DATABASE_URL "https://esp32-fd506-default-rtdb.firebaseio.com/"

void asyncCB(AsyncResult &aResult);

void printResult(AsyncResult &aResult);

void printError(int code, const String &msg);

DefaultNetwork network; // initilize with boolean parameter to enable/disable network reconnection

UserAuth user_auth(API_KEY, USER_EMAIL, USER_PASSWORD);

FirebaseApp app;

#if defined(ESP32) || defined(ESP8266) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFiClientSecure.h>
WiFiClientSecure ssl_client;
#elif defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_UNOWIFIR4) || defined(ARDUINO_GIGA) || defined(ARDUINO_OPTA) || defined(ARDUINO_PORTENTA_C33) || defined(ARDUINO_NANO_RP2040_CONNECT)
#include <WiFiSSLClient.h>
WiFiSSLClient ssl_client;
#endif

using AsyncClient = AsyncClientClass;

AsyncClient aClient(ssl_client, getNetwork(network));

RealtimeDatabase Database;

AsyncResult aResult_no_callback;

bool taskComplete = false;

void setup()
{

  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.printf("Firebase Client v%s\n", FIREBASE_CLIENT_VERSION);

  Serial.println("Initializing app...");

#if defined(ESP32) || defined(ESP8266) || defined(PICO_RP2040)
  ssl_client.setInsecure();
#if defined(ESP8266)
  ssl_client.setBufferSizes(4096, 1024);
#endif
#endif

  initializeApp(aClient, app, getAuth(user_auth), asyncCB, "authTask");

  // Binding the FirebaseApp for authentication handler.
  // To unbind, use Database.resetApp();
  app.getApp<RealtimeDatabase>(Database);

  Database.url(DATABASE_URL);

  Serial.println("Asynchronous Set... ");
}

void loop()
{
  // The async task handler should run inside the main loop
  // without blocking delay or bypassing with millis code blocks.

  app.loop();

  Database.loop();

  if (app.ready() && !taskComplete)
  {
    taskComplete = true;

    Serial.println("Asynchronous Set... ");

    // // Set int
    // Database.set<int>(aClient, "/test/int", 12345, asyncCB, "setIntTask");

    // // Set bool
    // Database.set<bool>(aClient, "/test/bool", true, asyncCB, "setBoolTask");

    // // Set string
    // Database.set<String>(aClient, "/test/string", "hello", asyncCB, "setStringTask");

    // // Set json
    // Database.set<object_t>(aClient, "/test/json", object_t("{\"data\":123}"), asyncCB, "setJsonTask");

    // Library does not provide JSON parser library, the following JSON writer class will be used with
    // object_t for simple demonstration.

    object_t json, obj1, obj2, obj3, obj4, ts_json;
    JsonWriter writer;

    //set string
  
    // Set JSON

    writer.create(ts_json, "timestamp/value/.sv", "timestamp");
    writer.create(obj2, "string/value", string_t("hello"));
    writer.join(obj4, 2 /* no. of object_t (s) to join */, obj2, ts_json);
    // writer.create(json, "/node", obj4);

    

    // To print object_t
    // Serial.println(json);
    Serial.println(obj4);

    // Database.set<object_t>(aClient, "/test/json", json, asyncCB, "setJsonTask");
    Database.set<object_t>(aClient, "/test/json", obj4, asyncCB, "setJsonTask");


    // // Set timestamp
    // writer.create(ts_json, ".sv", "timestamp"); // -> {".sv": "timestamp"}

    // Serial.println("Set only timestamp... ");
    // bool status = Database.set<object_t>(aClient, "/test/json", ts_json);
    // if (status)
    //   Serial.println(String("ok"));
    // else
    //   printError(aClient.lastError().code(), aClient.lastError().message());

    // object_t arr;
    // arr.initArray(); // initialize to be used as array
    // writer.join(arr, 4 /* no. of object_t (s) to join */, object_t("[12,34]"), object_t("[56,78]"), object_t(string_t("steve")), object_t(888));

    // Note that value that sets to object_t other than JSON ({}) and Array ([]) can be valid only if it
    // used as array member value as above i.e. object_t(string_t("steve")) and object_t(888).

    // // Set array
    // Database.set<object_t>(aClient, "/test/arr", arr, asyncCB, "setArrayTask");

    // // Set float
    // Database.set<number_t>(aClient, "/test/float", number_t(123.456, 2), asyncCB, "setFloatTask");

    // // Set double
    // Database.set<number_t>(aClient, "/test/double", number_t(1234.56789, 4), asyncCB, "setDoubleTask");

    // // Set timestamp
    // object_t ts_json;
    // writer.create(ts_json, ".sv", "timestamp"); // -> {".sv": "timestamp"}

    // Serial.println("Set only timestamp... ");
    // bool status = Database.set<object_t>(aClient, "/test/timestamp", ts_json);
    // if (status)
    //   Serial.println(String("ok"));
    // else
    //   printError(aClient.lastError().code(), aClient.lastError().message());

    //   object_t data_json, ts_data_json;

    //   writer.create(data_json, "data", "hello");        // -> {"data": "hello"}
    //   writer.join(ts_data_json, 2, data_json, ts_json); // -> {"data":"hello",".sv":"timestamp"}
  }
}

void asyncCB(AsyncResult &aResult)
{
  // WARNING!
  // Do not put your codes inside the callback and printResult.

  printResult(aResult);
}

void printResult(AsyncResult &aResult)
{
  if (aResult.isEvent())
  {
    Firebase.printf("Event task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.appEvent().message().c_str(), aResult.appEvent().code());
  }

  if (aResult.isDebug())
  {
    Firebase.printf("Debug task: %s, msg: %s\n", aResult.uid().c_str(), aResult.debug().c_str());
  }

  if (aResult.isError())
  {
    Firebase.printf("Error task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.error().message().c_str(), aResult.error().code());
  }

  if (aResult.available())
  {
    Firebase.printf("task: %s, payload: %s\n", aResult.uid().c_str(), aResult.c_str());
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// /**
//  * The complete usage guidelines, please visit https://github.com/mobizt/FirebaseClient
//  */

// #include <Arduino.h>
// #if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W) || defined(ARDUINO_GIGA) || defined(ARDUINO_OPTA)
// #include <WiFi.h>
// #elif defined(ESP8266)
// #include <ESP8266WiFi.h>
// #elif __has_include(<WiFiNINA.h>) || defined(ARDUINO_NANO_RP2040_CONNECT)
// #include <WiFiNINA.h>
// #elif __has_include(<WiFi101.h>)
// #include <WiFi101.h>
// #elif __has_include(<WiFiS3.h>) || defined(ARDUINO_UNOWIFIR4)
// #include <WiFiS3.h>
// #elif __has_include(<WiFiC3.h>) || defined(ARDUINO_PORTENTA_C33)
// #include <WiFiC3.h>
// #elif __has_include(<WiFi.h>)
// #include <WiFi.h>
// #endif

// #include <FirebaseClient.h>

// // #define WIFI_SSID "WIFI_AP"
// // #define WIFI_PASSWORD "WIFI_PASSWORD"

// // // The API key can be obtained from Firebase console > Project Overview > Project settings.
// // #define API_KEY "Web_API_KEY"

// // // User Email and password that already registerd or added in your project.
// // #define USER_EMAIL "USER_EMAIL"
// // #define USER_PASSWORD "USER_PASSWORD"
// // #define DATABASE_URL "URL"ran inne

// #define WIFI_SSID "Dialog Home"
// #define WIFI_PASSWORD "Work@home"

// // The API key can be obtained from Firebase console > Project Overview > Project settings.
// #define API_KEY "AIzaSyAA7sn8fyBh3LBH4Y7U5JYWepRmZGpoY8s"

// // User Email and password that already registerd or added in your project.
// #define USER_EMAIL "sameera824@gmail.com"
// #define USER_PASSWORD "Sameera@777"
// #define DATABASE_URL "https://esp32-fd506-default-rtdb.firebaseio.com/"

// void authHandler();

// void printResult(AsyncResult &aResult);

// void printError(int code, const String &msg);

// DefaultNetwork network; // initilize with boolean parameter to enable/disable network reconnection

// UserAuth user_auth(API_KEY, USER_EMAIL, USER_PASSWORD);

// FirebaseApp app;

// #if defined(ESP32) || defined(ESP8266) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
// #include <WiFiClientSecure.h>
// WiFiClientSecure ssl_client;
// #elif defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_UNOWIFIR4) || defined(ARDUINO_GIGA) || defined(ARDUINO_OPTA) || defined(ARDUINO_PORTENTA_C33) || defined(ARDUINO_NANO_RP2040_CONNECT)
// #include <WiFiSSLClient.h>
// WiFiSSLClient ssl_client;
// #endif

// using AsyncClient = AsyncClientClass;

// AsyncClient aClient(ssl_client, getNetwork(network));

// RealtimeDatabase Database;

// AsyncResult aResult_no_callback;

// void setup()
// {

//     Serial.begin(115200);
//     WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

//     Serial.print("Connecting to Wi-Fi");
//     while (WiFi.status() != WL_CONNECTED)
//     {
//         Serial.print(".");
//         delay(300);
//     }
//     Serial.println();
//     Serial.print("Connected with IP: ");
//     Serial.println(WiFi.localIP());
//     Serial.println();

//     Firebase.printf("Firebase Client v%s\n", FIREBASE_CLIENT_VERSION);

//     Serial.println("Initializing app...");

// #if defined(ESP32) || defined(ESP8266) || defined(PICO_RP2040)
//     ssl_client.setInsecure();
// #if defined(ESP8266)
//     ssl_client.setBufferSizes(4096, 1024);
// #endif
// #endif

//     initializeApp(aClient, app, getAuth(user_auth), aResult_no_callback);

//     authHandler();

//     // Binding the FirebaseApp for authentication handler.
//     // To unbind, use Database.resetApp();
//     app.getApp<RealtimeDatabase>(Database);

//     Database.url(DATABASE_URL);

//     // In case setting the external async result to the sync task (optional)
//     // To unset, use unsetAsyncResult().
//     aClient.setAsyncResult(aResult_no_callback);

//     // Library does not provide JSON parser library, the following JSON writer class will be used with
//     // object_t for simple demonstration.

//     object_t ts_json;
//     JsonWriter writer;
//     writer.create(ts_json, ".sv", "timestamp"); // -> {".sv": "timestamp"}

//     Serial.println("Set only timestamp... ");
//     bool status = Database.set<object_t>(aClient, "/test/timestamp", ts_json);
//     if (status)
//         Serial.println(String("ok"));
//     else
//         printError(aClient.lastError().code(), aClient.lastError().message());

//     object_t data_json, ts_data_json;

//     writer.create(data_json, "data", "hello");        // -> {"data": "hello"}
//     writer.join(ts_data_json, 2, data_json, ts_json); // -> {"data":"hello",".sv":"timestamp"}

//     Serial.println("Set timestamp and data... ");
//     status = Database.set<object_t>(aClient, "/test/timestamp", ts_data_json);
//     if (status)
//         Serial.println(String("ok"));
//     else
//         printError(aClient.lastError().code(), aClient.lastError().message());
// }

// void loop()
// {
//     authHandler();

//     Database.loop();
// }

// void authHandler()
// {
//     // Blocking authentication handler with timeout
//     unsigned long ms = millis();
//     while (app.isInitialized() && !app.ready() && millis() - ms < 120 * 1000)
//     {
//         // The JWT token processor required for ServiceAuth and CustomAuth authentications.
//         // JWT is a static object of JWTClass and it's not thread safe.
//         // In multi-threaded operations (multi-FirebaseApp), you have to define JWTClass for each FirebaseApp,
//         // and set it to the FirebaseApp via FirebaseApp::setJWTProcessor(<JWTClass>), before calling initializeApp.
//         JWT.loop(app.getAuth());
//         printResult(aResult_no_callback);
//     }
// }
// void printResult(AsyncResult &aResult)
// {
//     if (aResult.isEvent())
//     {
//         Firebase.printf("Event task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.appEvent().message().c_str(), aResult.appEvent().code());
//     }

//     if (aResult.isDebug())
//     {
//         Firebase.printf("Debug task: %s, msg: %s\n", aResult.uid().c_str(), aResult.debug().c_str());
//     }

//     if (aResult.isError())
//     {
//         Firebase.printf("Error task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.error().message().c_str(), aResult.error().code());
//     }
// }

void printError(int code, const String &msg)
{
  Firebase.printf("Error, msg: %s, code: %d\n", msg.c_str(), code);
}