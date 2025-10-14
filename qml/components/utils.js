function writeJson(path, data) {
    const file = Qt.openUrlExternally; // dummy to check existence
    const io = require("io");
    try {
        const f = io.open(path, "w");
        f.write(JSON.stringify(data, null, 4));
        f.close();
        console.log("✅ Saved JSON to", path);
    } catch (e) {
        console.error("❌ Error writing JSON:", e);
    }
}

function readJson(path) {
    const io = require("io");
    try {
        const f = io.open(path, "r");
        const content = f.readAll();
        f.close();
        return JSON.parse(content);
    } catch (e) {
        console.log("⚠ Could not read JSON:", e);
        return [];
    }
}
