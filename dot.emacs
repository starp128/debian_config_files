
(add-to-list 'load-path "~/.emacs_config")
(add-to-list 'load-path "~/.emacs_config/ecb-2.32/")


;;(customize-set-variable 'scroll-bar-mode' right)
;;取消滚动栏
;;(set-scroll-bar-mode nil)
(column-number-mode t)
;;hide menu bar
(menu-bar-mode nil)
;;(setq default-tab-width 4)
(setq inhibit-startup-message t)
(display-time-mode 1)

;; 实现全屏效果，快捷键为f11
(global-set-key [f11] 'my-fullscreen) 
(defun my-fullscreen ()
(interactive)
(x-send-client-message
nil 0 nil "_NET_WM_STATE" 32
'(2 "_NET_WM_STATE_FULLSCREEN" 0))
)

;; 最大化
;; (defun my-maximized ()
;; (interactive)
;; (x-send-client-message
;; nil 0 nil "_NET_WM_STATE" 32
;; '(2 "_NET_WM_STATE_MAXIMIZED_HORZ" 0))
;; (x-send-client-message
;; nil 0 nil "_NET_WM_STATE" 32
;; '(2 "_NET_WM_STATE_MAXIMIZED_VERT" 0))
;; )

;; 启动emacs时窗口最大化
;;(my-maximized)

;;shutdown "~*" backup file
;;(setq auto-save-default nil)

;;默认的路径
(setq default-directory "/home/guyi/ws")

;;设置M-/作为标志位，默认C-@来setmark,C-@不太好用
;;(global-set-key (kbd "M-/") 'set-mark-command)

;;改变Emacs要你回答yes的行为。按y或空格键表示yes，n表示no。
(fset 'yes-or-no-p 'y-or-n-p)

;;禁止在鼠标点击的地方插入剪贴板内容
(setq mouse-yank-at-point t)

;;-----------------------------

;; (cond window-system 
;;       (require 'func-menu)
;;       (define-key global-map [S-down-mouse-3] 
;; 	'function-menu))

(require 'func-menu)
(define-key global-map [f5] 'function-menu)



;; -----------------------------
;; 屏幕滚动光标不动
(global-set-key [(meta n)] (lambda (&optional n) (interactive "p")
(scroll-up (or n 1))))

(global-set-key [(meta p)] (lambda (&optional n) (interactive "p")
(scroll-down (or n 1)))) 

;;----------------------------------------------------------------------------------------------------------------------+

;;Yeah , I found this bug too. It seems that the Emacs 24 dosen't have the ' plist-to-alist ' function. So probably you should write it yourself. Here is mine. Put this function in your dot-emacs file then it will be ok.
(defun plist-to-alist (the-plist)
 (defun get-tuple-from-plist (the-plist)
   (when the-plist
     (cons (car the-plist) (cadr the-plist))))

 (let ((alist '()))
   (while the-plist
     (add-to-list 'alist (get-tuple-from-plist the-plist))
     (setq the-plist (cddr the-plist)))
 alist))


(require 'color-theme)
(color-theme-arjen)

(require 'setnu)
(setnu-mode t)
(global-set-key [f3] (quote setnu-mode))

 (require 'highlight-symbol)
(global-set-key [(control f4)] 'highlight-symbol-at-point)
 (global-set-key [f4] 'highlight-symbol-next)
 (global-set-key [(shift f4)] 'highlight-symbol-prev)
;; (global-set-key [(meta f4)] 'highlight-symbol-prev)))     ;;confilct with wm
;; (global-set-key [(control meta f4)] 'highlight-symbol-query-replace)

;;----------------------------------------------------------------------------------------------------------------------+
;;1 highlight word  under cruser in current file
;;2 jump to defination
;;  imenu
;;   etags    ;;visit-tag-table
;;    cscopes
;;    ecb
;;3 complete
;; M-/  dabbre-expand    expand pre-used word
;;  standalone cedet cann't be installed
;;   C-c , SPC in shipped semantic of cedet 


;;4 look like IDE
(require 'tabbar)
(tabbar-mode t)
(require 'sr-speedbar);;speedbar in current frame  .download from emacswiki.org
;;copy from frankyxhl(https://gist.github.com/frankyxhl/3780388)
(custom-set-variables
'(speedbar-show-unknown-files t)
)
;;sr-speedbar-right-side 把speedbar放在左侧位置
;;sr-speedbar-skip-other-window-p 多窗口切换时跳过speedbar窗口
;;sr-speedbar-max-width与sr-speedbar-width-x 设置宽度
;;(custom-set-variables '(sr-speedbar-right-side nil))
(custom-set-variables '(sr-speedbar-skip-other-window-p t))
(custom-set-variables '(sr-speedbar-width-x 5))
(custom-set-variables '(sr-speedbar-max-width 10))

;; 绑定快捷键
;;(global-set-key (kbd "s-s") 'sr-speedbar-toggle)
;;(global-set-key (kbd "s-r") 'sr-speedbar-refresh-toggle)

(add-hook 'after-init-hook '(lambda () (sr-speedbar-toggle)));;auto start sr-speedbar
(line-number-mode t)
(add-hook 'speedbar-mode-hook '(lambda () (linum-mode -1)))
;;(require 'ecb)
;;imenu
(global-set-key (kbd "<C-down-mouse-1>") nil) ;; 去掉原来的帮定关系
;;(global-set-key (kbd "<C-mouse-1>") 'mouse-buffer-menu)
(global-set-key (kbd "<C-mouse-1>") 'imenu)
;;(global-set-key [C-mouse-3] 'imenu)) ; GNU Emacs


;;----------------------------------------------------------------------------------------------------------------------+

;;==shortcut for etags++==============================================================
;;(setq tags-file-name "/home/guyi/ws/kernel/linux-2.6.28/TAGS")

;;(global-set-key [(f7)] 'visit-tags-table)         ; visit tags table  
;;(global-set-key [C-f7] 'sucha-generate-tag-table) ; generate tag table  
;;开个小窗查看光标处的 tag
;;(global-set-key [(control .)] '(lambda () (interactive) (lev/find-tag t)))  
;;只留下当前查看代码的窗口（关闭查看 tag 的小窗）
;;(global-set-key [(control ,)] 'sucha-release-small-tag-window)  
;;(global-set-key [(meta .)] 'lev/find-tag)  
;;(global-set-key [(meta ,)] 'pop-tag-mark) 
 
;;(global-set-key (kbd "C-M-,") 'find-tag)  
;;Shift-Tab, C/C++ 和 lisp 等模式中补全函数名（一般情况下M-Tab被窗口管理器遮屏了）
(define-key lisp-mode-shared-map [(shift tab)] 'complete-tag)  
(add-hook 'c-mode-common-hook      ; both c and c++ mode  
	  (lambda ()  
	    (define-key c-mode-base-map [(shift tab)] 'complete-tag)))  

(defun lev/find-tag (&optional show-only)  
  "Show tag in other window with no prompt in minibuf."  
  (interactive)  
  (let ((default (funcall (or find-tag-default-function  
                              (get major-mode 'find-tag-default-function)  
                              'find-tag-default))))  
    (if show-only  
        (progn (find-tag-other-window default)  
               (shrink-window (- (window- height) 12)) ;; 限制为 12 行  
               (recenter 1)  
               (other-window 1))  
      (find-tag default))))  
  
(defun sucha-generate-tag-table ()  
  "Generate tag tables under current directory(Linux)."  
  (interactive)  
  (let   
      ((exp "")  
       (dir ""))  
    (setq dir  
          (read-from-minibuffer "generate tags in: " default-directory)  
          exp  
          (read-from-minibuffer "suffix: "))  
    (with-temp-buffer  
      (shell-command  
       (concat "find " dir " -name /"" exp "/" | xargs etags ")  
       (buffer-name)))))  
  
(defun sucha-release-small-tag-window ()  
  "Kill other window also pop tag mark."  
  (interactive)  
  (delete-other-windows)  
  (ignore-errors  
    (pop-tag-mark)))  
;;end shortcut for etag========================================================================
;;todo disable  highlight active region
;;auto highlight parentheres
