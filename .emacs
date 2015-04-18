(add-to-list 'load-path "~/.emacs_config")

(menu-bar-mode -1)
(column-number-mode t)
(fset 'yes-or-no-p 'y-or-n-p)
(show-paren-mode t)

;;make window splited side by side when using C-x 4 prefix
;;'nil means do not enable split vertically,(up and down).
(setq split-height-threshold nil)
;;without this limit, window will be created too narrow, the default may be OK.
(setq split-width-threshold 80)

;;leave this to tmux for send prefix key
(global-unset-key (kbd "C-z"))

(require 'highlight-symbol)
(global-set-key [f6] 'highlight-symbol-at-point)
(global-set-key [f7] 'highlight-symbol-next)
(global-set-key [f5] 'highlight-symbol-prev)

(defun terisk-scroll-up (&optional n)
""
(interactive "p")
(scroll-up (or n 1)))

(defun terisk-scroll-down (&optional n)
""
(interactive "p")
(scroll-down (or n 1)))

(defun terisk-scroll-other-window-up ()
""
(interactive)
(scroll-other-window 1))

(defun terisk-scroll-other-window-down ()
""
(interactive)
(scroll-other-window -1))

(defun terisk-scroll-other-window-up-whole-frame ()
""
(interactive)
(scroll-other-window))

(defun terisk-scroll-other-window-down-whole-frame ()
""
(interactive)
(scroll-other-window '-))


;;
(define-minor-mode terisk-mode
  "I define some hot key in this minor mode, so the hot key I defined
can always overlap that defined in other keymaps. These key main involvs
navigation"
  :lighter " tt"
  :keymap (let ((map (make-sparse-keymap)))
            (define-key map (kbd "M-n") 'terisk-scroll-up)
            (define-key map (kbd "M-p") 'terisk-scroll-down)
	    (define-key map (kbd "M-K") 'terisk-scroll-other-window-up)
	    (define-key map (kbd "M-L") 'terisk-scroll-other-window-down)
	    (define-key map (kbd "M-V") 'terisk-scroll-other-window-up-whole-frame)
	    (define-key map (kbd "M-P") 'terisk-scroll-other-window-down-whole-frame)
            map))
(provide 'foo-mode)

(global-set-key (kbd "C-t") 'terisk-mode)

(defun tough-insert-buffer-region-to-end-within-this-buffer (dstbuffer)
  "like the function before, but stay in the src buffer"
  (interactive "*bwhich is the dst buffer")
  (let (tmp-mark tmp-point tmp-buffer)
    (setq tmp-point (point))
    (setq tmp-mark (mark))
    (setq tmp-buffer (current-buffer))
    (save-excursion
      (set-buffer (get-buffer dstbuffer))
      (goto-char (point-max))
      (insert ?\n)
      (insert-buffer-substring tmp-buffer tmp-mark tmp-point))))
(defun tough-insert-buffer-region-to-end (buffer)
  "insert region of selected buffer at the end of current buffer"
  (interactive "*bwhich is the source buffer")
  (let (tmp-mark tmp-point tmp-current-point)
    (save-excursion
      (set-buffer (get-buffer buffer))
      (setq tmp-mark (mark))
      (setq tmp-point (point))
      )
    (save-excursion
      (goto-char (point-max))
      (insert ?\n)
      (insert-buffer-substring (get-buffer buffer) tmp-mark tmp-point)
      (setq tmp-current-point (point))
      )
    (push-mark tmp-current-point)
    )
  )

(setq c-default-style '((java-mode . "java")
			(awk-mode . "awk")
			(other . "linux")))

(global-unset-key [C-z])

(set-face-attribute  'mode-line
                 nil
                 :foreground "white"
                 :background "black"
                 :box '(:line-width 1 :style released-button))

(set-face-attribute  'mode-line-inactive
                 nil
                 :foreground "gray75"
                 :background "gray25"
                 :box '(:line-width 1 :style released-button))

;;(load (expand-file-name "~/quicklisp/slime-helper.el"))
;; Replace "sbcl" with the path to your implementation
;;(setq inferior-lisp-program "sbcl")
