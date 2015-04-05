(in-package :asdf-user)
(defsystem libfixposix
  :version "0.1"
  :author "Masataro Asai"
  :mailto "guicho2.71828@gmail.com"
  :license "MIT"
  :components ((:module :build
                :perform
                (compile-op (o c)
                            (run-shell-command "../configure")
                            (run-shell-command "make")))))

(defmethod perform :before ((op compile-op) (c (eql (find-system 'libfixposix))))
  (run-shell-command "autoreconf -i -f"))

