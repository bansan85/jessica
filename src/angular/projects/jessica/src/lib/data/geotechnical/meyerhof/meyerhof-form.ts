/* eslint-disable max-len */
import { FoundationStrip } from '../foundation-strip/foundation-strip';
import { VerticalEccentric } from '../../load/vertical-eccentric/vertical-eccentric';
/* eslint-enable max-len */

export interface MeyerhofForm {
  foundation: FoundationStrip;
  load: VerticalEccentric;
}
