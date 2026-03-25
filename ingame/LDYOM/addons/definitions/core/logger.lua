---@meta

log = {}

---Log a trace message
---@param msg string The message to log
function log.trace(msg) end

---Log a debug message
---@param msg string The message to log
function log.debug(msg) end

---Log an info message
---@param msg string The message to log
function log.info(msg) end

---Log a warning message
---@param msg string The message to log
function log.warn(msg) end

---Log an error message
---@param msg string The message to log
function log.error(msg) end

---Log a critical message
---@param msg string The message to log
function log.critical(msg) end

---Log a formatted trace message
---@param format string The format string
---@vararg string The values to format
function log.tracef(format, ...) end

---Log a formatted debug message
---@param format string The format string
---@vararg string The values to format
function log.debugf(format, ...) end

---Log a formatted info message
---@param format string The format string
---@vararg string The values to format
function log.infof(format, ...) end

---Log a formatted warning message
---@param format string The format string
---@vararg string The values to format
function log.warnf(format, ...) end

---Log a formatted error message
---@param format string The format string
---@vararg string The values to format
function log.errorf(format, ...) end

---Log a formatted critical message
---@param format string The format string
---@vararg string The values to format
function log.criticalf(format, ...) end