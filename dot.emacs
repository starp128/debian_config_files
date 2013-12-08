(add-to-list 'load-path "/root/my_emacs_config")

(require 'color-theme)
(color-theme-arjen)

(require 'setnu)
(setnu-mode t)
(global-set-key [f3] (quote setnu-mode))