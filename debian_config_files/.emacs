(add-to-list 'load-path "/b/.cnt/debian_config_files/emacs.d/")

(menu-bar-mode -1)
(column-number-mode t)
(fset 'yes-or-no-p 'y-or-n-p)
(show-paren-mode t)
(ido-mode t)

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
can always overlap that defined in other keymaps. These key mainly involves
navigation"
  :lighter " tt"
  :keymap (let ((map (make-sparse-keymap)))
            (define-key map (kbd "M-n") 'terisk-scroll-up)
            (define-key map (kbd "M-p") 'terisk-scroll-down)
	    (define-key map (kbd "M-K") 'terisk-scroll-other-window-up)
	    (define-key map (kbd "M-L") 'terisk-scroll-other-window-down)
	    (define-key map (kbd "M-V") 'terisk-scroll-other-window-up-whole-frame)
	    (define-key map (kbd "M-P") 'terisk-scroll-other-window-down-whole-frame)
	    (define-key map (kbd "C-c j t") 'terisk-note-time)
	    (define-key map (kbd "C-c j n") 'terisk-note-entry-2)
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

(which-func-mode)

(define-skeleton terisk-note-time
  "insert main entry"
  nil
  > "<DAY time=\"" (format-time-string "%Y-%m-%d-%a") "\">" \n _ \n
  > "</DAY>">)

(define-skeleton terisk-note-entry
  "insert sub entry"
  "what element(tips log todo): "
  > "<" str " tags=\"" _ "\">" \n \n
  "</" str ">" >)

(define-skeleton terisk-note-entry-2
  "insert sub entry"
  ""
  '(setq str (skeleton-read "choose one([tips] log todo) : "))
  '(when (string= str "") (setq str "tips"))
  > "<" str " tags=\"" _ "\">" \n \n
  "</" str ">" >)

;;#000000 black
;;#ffffff white
;;#aabbcc aa-red bb-green cc-blue
;;list-colors-display
(set-face-foreground 'mode-line "black")
(set-face-background 'mode-line "green")
(set-face-foreground 'mode-line-inactive "black")
(set-face-background 'mode-line-inactive "cyan")


(custom-set-variables
  ;; custom-set-variables was added by Custom.
  ;; If you edit it by hand, you could mess it up, so be careful.
  ;; Your init file should contain only one such instance.
  ;; If there is more than one, they won't work right.
 '(auto-insert-mode t))

    ;;; integrate ido with artist-mode
(defun artist-ido-select-operation (type)
  "Use ido to select a drawing operation in artist-mode"
  (interactive (list (ido-completing-read "Drawing operation: "
					  (list "Pen" "Pen Line" "line" "straight line" "rectangle"
						"square" "poly-line" "straight poly-line" "ellipse"
						"circle" "text see-thru" "text-overwrite" "spray-can"
						"erase char" "erase rectangle" "vaporize line" "vaporize lines"
						"cut rectangle" "cut square" "copy rectangle" "copy square"
						"paste" "flood-fill"))))
  (artist-select-operation type))

(defun artist-ido-select-settings (type)
  "Use ido to select a setting to change in artist-mode"
  (interactive (list (ido-completing-read "Setting: "
					  (list "Set Fill" "Set Line" "Set Erase" "Spray-size" "Spray-chars"
						"Rubber-banding" "Trimming" "Borders"))))
  (if (equal type "Spray-size")
      (artist-select-operation "spray set size")
    (call-interactively (artist-fc-get-fn-from-symbol
			 (cdr (assoc type '(("Set Fill" . set-fill)
					    ("Set Line" . set-line)
					    ("Set Erase" . set-erase)
					    ("Rubber-banding" . rubber-band)
					    ("Trimming" . trimming)
					    ("Borders" . borders)
					    ("Spray-chars" . spray-chars))))))))

(add-hook 'artist-mode-init-hook 
	  (lambda ()
	    (define-key artist-mode-map (kbd "C-c C-a C-o") 'artist-ido-select-operation)
	    (define-key artist-mode-map (kbd "C-c C-a C-c") 'artist-ido-select-settings)))


(setq dired-listing-switches "-al --group-directories-first")
