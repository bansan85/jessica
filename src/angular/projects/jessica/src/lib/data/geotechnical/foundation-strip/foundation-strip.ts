import { I18nForm } from 'toolbox';

export interface FoundationStrip<T extends number | string> {
  width: T;
}

export class FoundationStripI18n implements I18nForm {
  public number: string[] = ['width'];
}
