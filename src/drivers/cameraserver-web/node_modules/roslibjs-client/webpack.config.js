module.exports = {
    context: __dirname + "/src",
    entry: "./browser",
    output: {
        path: __dirname + "/build",
        filename: "roslibjs-client.js"
    }
};