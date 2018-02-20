var mesh = require("mesh");

module.exports = function() {
  return mesh.tailable(mesh.fallback(
    ping()
  ), function() { return true; });
};

function ping() {
  return mesh.accept("ping", mesh.wrap(function(operation, next) {
    setTimeout(next, Math.random() * 1000, void 0, operation.data || true);
  }));
}
