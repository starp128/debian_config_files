;;emacs configuration file
;;rename it to ".emacs" and put it in your home folder

(add-to-list 'load-path "~/.emacs_config")

(require 'color-theme)
(color-theme-arjen)

(require 'setnu)
(setnu-mode t)
(global-set-key [f3] (quote setnu-mode))

(column-number-mode t)

;;hide menu bar
(menu-bar-mode nil)

(setq default-tab-width 4)