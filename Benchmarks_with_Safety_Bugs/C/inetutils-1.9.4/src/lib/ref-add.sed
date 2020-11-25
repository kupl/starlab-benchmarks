/^# Packages using this file: / {
  s/# Packages using this file://
  ta
  :a
  s/ inetutils / inetutils /
  tb
  s/ $/ inetutils /
  :b
  s/^/# Packages using this file:/
}
