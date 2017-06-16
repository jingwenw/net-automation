(load-file "/usr/share/emacs/site-lisp/xcscope/xcscope.el")
(require 'xcscope)

;; inhibit-startup-echo-area-message MUST be set to a hardcoded
;; string of your login name

(when window-system (set-frame-size (selected-frame) 80 50))

(setq inhibit-startup-echo-area-message "jwang")
(setq inhibit-startup-message t)
(setq inhibit-startup-screen t)

(setq c-basic-offset 4) ; indents 4 chars
(setq tab-width 4)          ; and 4 char wide for TAB
(setq indent-tabs-mode nil) ; And force use of spaces

(display-time)
(line-number-mode 1)
(column-number-mode 1)

(global-font-lock-mode t)
(setq font-lock-maximum-size nil)

(setq auto-mode-alist
      (append '(
               ("configure.in" . m4-mode)
               ("\\.m4\\'" . m4-mode)
               ("\\.am\\'" . makefile-mode))
              auto-mode-alist))

(add-hook 'dired-load-hook (function (lambda () (load "dired-x"))))
(setq dired-omit-files-p t)

(global-set-key [mouse-2] 'yank)


;;(setq cscope-initial-directory "/workspace/git/ejenwng/ipos.lsv")

(define-key global-map [(ctrl f3)] 'cscope-set-initial-directory)
(define-key global-map [(ctrl f4)] 'cscope-unset-initial-directory)

(define-key global-map [(ctrl f5)] 'cscope-find-this-symbol)

(define-key global-map [(ctrl f6)] 'cscope-find-global-definition)

(define-key global-map [(ctrl f7)] 'cscope-find-global-definition-no-prompting)

(define-key global-map [(ctrl f8)] 'cscope-pop-mark)

(define-key global-map [(ctrl f9)] 'cscope-next-symbol)

(define-key global-map [(ctrl f10)] 'cscope-next-file)

(define-key global-map [(ctrl f11)] 'cscope-prev-symbol)

(define-key global-map [(ctrl f12)] 'cscope-prev-file)

(define-key global-map [(meta f9)] 'cscope-display-buffer)

(define-key global-map [(meta f10)] 'cscope-display-buffer-toggle)

(put 'upcase-region 'disabled nil)

(defun my-c-mode-hook ()
(setq c-basic-offset 4)
(setq indent-tabls-mode nil)
(c-set-offset 'case-label '+))
(add-hook 'c-mode-hook 'my-c-mode-hook)

(setq whitespace-space 'underline)

(setq-default indent-tabs-mode nil)
(setq-default show-trailing-whitespace t)

;; (add-hook 'before-save-hook 'delete-trailing-whitespace)
;; (add-hook 'before-save-hook 'whitespace-cleanup)

(let ((d (make-display-table)))
  (aset d 9 (vector ?> ?>))
  (set-window-display-table nil d))
